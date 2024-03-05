local use_camera = false;
local camera_sens = 0.08;

function clamp(x, min, max)
    if x < min then return min end
    if x > max then return max end
    return x
end

function on_init()
    Input.set_cursor_state(CursorState.Normal);
end

function on_update(dt)
    local tc = owner:get(this, TransformComponent);
    local cc = owner:get(this, CameraComponent);
    local final_yaw_pitch = Vec2.new(tc.rotation.y, tc.rotation.x);

    if Input.get_key_pressed(Key.Escape) then
        use_camera = not use_camera;
    end

    if use_camera then
        Input.set_cursor_state(CursorState.Disabled);
        local new_mouse_position = Input.get_mouse_position();

        local change = new_mouse_position * camera_sens;
        final_yaw_pitch.x = final_yaw_pitch.x + change.x;
        final_yaw_pitch.y = clamp(final_yaw_pitch.y - change.y, -89.9, 89.9);
        tc.rotation.x = math.rad(final_yaw_pitch.y);
        tc.rotation.y = math.rad(final_yaw_pitch.x);
    else
        Input.set_cursor_state(CursorState.Normal);
    end

    local speed = 0.01;

    if Input.get_key_held(Key.W) then
        if use_camera then
            tc.position = tc.position + (cc.camera:get_forward() * speed * dt);
        else
            tc.position.x = tc.position.x + (speed * dt);
        end
    end

    if Input.get_key_held(Key.S) then
        if use_camera then
            tc.position = tc.position - (cc.camera:get_forward() * speed * dt);
        else
            tc.position.x = tc.position.x - (speed * dt);
        end
    end

    if Input.get_key_held(Key.D) then
        tc.position = tc.position + (cc.camera:get_right() * speed * dt);
    end

    if Input.get_key_held(Key.A) then
        tc.position = tc.position - (cc.camera:get_right() * speed * dt);
    end
end
