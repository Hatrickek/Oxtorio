function is_point_inside(aabb, point)
    if (point.x < aabb.min.x or point.x > aabb.max.x or point.z < aabb.min.z or point.z > aabb.max.z) then
        return false;
    end
    return true;
end

function on_update(dt)
    local belt_view = owner:runtime_view(BeltComponent);
    belt_view:each(function(belt_entity)
        local belt_position = owner:get(belt_entity, TransformComponent).position;
        local belt_aabb = owner:get(belt_entity, MeshComponent).aabb;

        local pickable_view = owner:runtime_view(PickableComponent);
        pickable_view:each(function(pickable_entity)
            local pickable_position = owner:get(pickable_entity, TransformComponent).position;
            local pickable_aabb = owner:get(pickable_entity, MeshComponent).aabb;
            if is_point_inside(belt_aabb, pickable_position) then
                pickable_position.x = belt_position.x;
                pickable_position.z = belt_position.z;
                pickable_position.y = belt_position.y + pickable_aabb:get_size().y;
            end
        end)
    end)
end
