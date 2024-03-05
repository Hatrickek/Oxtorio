local build_mode = false;
local remove_mode = false;
local place_entity = nil;
local can_place = false;

local build_material = Material.new("BuildMaterial");
local default_material = Material.new("MinerMaterial");

local hotbar_hovered = false;

local last_rotation = 0.0;

function create_ore()
    local path = App.get_absolute("Objects/iron_ore.glb");
    local mesh = Assets.get_mesh(path);
    local ore_entity = scene:load_mesh(mesh);
    owner:emplace(ore_entity, OreComponent.new(OreType.Iron));
    return ore_entity;
end

function on_init()
    build_material:set_color(Vec4.new(1, 1, 1, 0.5));
    build_material:set_alpha_mode(AlphaMode.Blend);

    for row = 0, 5, 1 do
        for col = 0, 5, 1 do
            local ore_entity = create_ore();
            local tc = owner:get(ore_entity, TransformComponent);
            tc.position.x = row;
            tc.position.z = col;
            tc.rotation.y = math.random();
        end
    end
end

function create_place_entity(component, mesh_path)
    local mesh = Assets.get_mesh(mesh_path);
    place_entity = scene:load_mesh(mesh);
    default_material = owner:get(place_entity, MeshComponent).materials[1];
    owner:get(place_entity, TagComponent).tag = "placeable";
    owner:emplace(place_entity, PlaceableComponent.new(false));
    owner:emplace(place_entity, component);
end

function is_point_inside(aabb, point)
    if (point.x < aabb.min.x or point.x > aabb.max.x or point.z < aabb.min.z or point.z > aabb.max.z) then
        return false;
    end
    return true;
end

function handle_hit(ray, hit)
    local hit_point = ray:get_point_on_ray(hit.fraction);

    local selection_box = AABB.new(Vec3.new(-0.5, -0.5, -0.5), Vec3.new(0.5, 0.5, 0.5));

    -- just use debug util for now to draw a selection cube :)
    if build_mode or remove_mode then
        local pos = Vec3.new(hit_point.x, hit_point.y, hit_point.z); -- TODO need a copy constructor for Vec types
        pos = glm.round(pos);
        pos.y = pos.y + 0.5;
        selection_box:transform(glm.translate(Mat4.new(1.0), pos));
        Debug.draw_aabb(selection_box, Vec3.new(1.0, 0.45, 0), true);
    end

    if build_mode and place_entity then
        local tc = owner:get(place_entity, TransformComponent);
        local mc = owner:get(place_entity, MeshComponent);
        tc.position = hit_point;
        tc.position.x = glm.round(tc.position.x);
        tc.position.y = tc.position.y + mc.aabb:get_size().y * 0.5;
        tc.position.z = glm.round(tc.position.z);

        if Input.get_key_pressed(Key.R) then
            last_rotation = last_rotation + math.rad(90);
        end

        tc.rotation.y = last_rotation;

        if Input.get_mouse_clicked(MouseButton.Left) and can_place and not hotbar_hovered then
            default_material:set_alpha_mode(AlphaMode.Opaque);
            default_material:set_color(Vec4.new(1, 1, 1, 1));
            default_material:set_emissive(Vec4.new(0, 0, 0, 0));
            mc.materials[1] = default_material;
            place_entity = nil;
            can_place = false;
        end
    end

    if remove_mode then
        if Input.get_mouse_clicked(MouseButton.Right) and remove_mode then
            -- TODO temporary until we can get entity info from rayhits
            local view = owner:runtime_view(PlaceableComponent);
            view:each(function(e)
                local position = owner:get(e, TransformComponent).position;
                if selection_box:is_point_inside(position) ~= Intersection.Outside then
                    owner:destroy(e);
                    return;
                end
            end)
        end
    end
end

function on_update(dt)
    if build_mode then
        can_place = true;

        -- check the intersection with all placables in the scene
        local placeable_mesh_view = owner:runtime_view(MeshComponent, PlaceableComponent);
        placeable_mesh_view:each(function(mesh_entity)
            if place_entity ~= mesh_entity then
                local pos = owner:get(place_entity, TransformComponent).position;
                local aabb = owner:get(mesh_entity, MeshComponent).aabb;
                if is_point_inside(aabb, pos) then
                    can_place = false;
                end
            end
        end)

        if place_entity then
            -- check if miner is ontop of an ore then set it's type to that ore
            if owner:has(place_entity, MinerComponent) and can_place then
                can_place = false;
                local ore_view = owner:runtime_view(OreComponent);
                ore_view:each(function(ore_entity)
                    local pos = owner:get(place_entity, TransformComponent).position;
                    local aabb = owner:get(ore_entity, MeshComponent).aabb;
                    if is_point_inside(aabb, pos) then
                        can_place = true;
                        owner:get(place_entity, MinerComponent).type = owner:get(ore_entity, OreComponent).type;
                    end
                end)
            end
            
            -- set materials
            if can_place then
                build_material:set_emissive(Vec4.new(0, 1, 0, 0.5));
                owner:get(place_entity, MeshComponent).materials[1] = build_material;
            else
                build_material:set_emissive(Vec4.new(1, 0, 0, 0.5));
                owner:get(place_entity, MeshComponent).materials[1] = build_material;
            end
        end
    end

    if build_mode or remove_mode then
        local mouse_position = Input.get_mouse_position();
        mouse_position = mouse_position - Renderer.get_viewport_offset();
        local camera_ent = owner:runtime_view(CameraComponent):front();
        local camera_component = owner:get(camera_ent, CameraComponent);
        local ray = camera_component.camera:get_screen_ray(mouse_position);
        ray = RayCast.new(ray:get_origin(), ray:get_direction() * 30);
        --Debug.draw_ray(ray, Vec3.new(1, 1, 1));
        local hit_collector = Physics.cast_ray(ray);
        local hit_results = Physics.get_hits(hit_collector);
        local hit = hit_results[1];

        if hit then
            handle_hit(ray, hit);
        end
    end
end

function on_imgui_render()
    local open = true;
    if ImGui.Begin("haiii", open, ImGuiWindowFlags.AlwaysAutoResize) then
        hotbar_hovered = ImGui.IsWindowHovered();

        if ImGui.Button("build mode") then
            build_mode = not build_mode;
            remove_mode = false;
            if place_entity then
                owner:destroy(place_entity);
                place_entity = nil;
            end
        end

        ImGui.SameLine();
        if ImGui.Button("remove mode") then
            remove_mode = not remove_mode;
            build_mode = false;
            if place_entity then
                owner:destroy(place_entity);
                place_entity = nil;
            end
        end

        if build_mode then
            if ImGui.Button("Miner") then
                local path = App.get_absolute("Objects/miner.glb");

                if not place_entity then
                    create_place_entity(MinerComponent.new(), path);
                else
                    owner:destroy(place_entity);
                    place_entity = nil;
                end
            end
            ImGui.SameLine();
            if ImGui.Button("Belt") then
                local path = App.get_absolute("Objects/belt.glb");

                if not place_entity then
                    create_place_entity(BeltComponent.new(1.0), path);
                else
                    owner:destroy(place_entity);
                    place_entity = nil;
                end
            end
        end

        ImGui.End();
    end
end
