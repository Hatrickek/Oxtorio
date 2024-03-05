function create_mined_ore(ore_type)
    local path = App.get_absolute("Objects/box.glb");
    local mesh = Assets.get_mesh(path);
    local ore_entity = scene:load_mesh(mesh);
    owner:emplace(ore_entity, PickableComponent.new());
    return ore_entity;
end

local timer = 0.0;

function on_update(dt)
    timer = timer + dt;
    local miner_view = owner:runtime_view(MinerComponent);
    miner_view:each(function(miner_entity)
        local miner_tc = owner:get(miner_entity, TransformComponent);
        local miner_component = owner:get(miner_entity, MinerComponent);
        if timer > miner_component.speed then
            timer = 0.0;
            local ore = create_mined_ore(miner_component.type);
            local ore_tc = owner:get(ore, TransformComponent);

            if ore_tc.rotation.y == 0.0 then
                ore_tc.position.x = miner_tc.position.x - 1;
                Log.info(owner:get(ore, MeshComponent).aabb:get_size().y)
                ore_tc.position.y = owner:get(ore, MeshComponent).aabb:get_size().y;
                ore_tc.position.z = miner_tc.position.z;
            end
        end
    end)
end
