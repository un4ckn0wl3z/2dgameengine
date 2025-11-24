-- Define a table with the values of the first leve
local current_system_hour = os.date("*t").hour
local map_texture_asset_id
if current_system_hour >= 9 and current_system_hour <= 18 then
    map_texture_asset_id = "tilemap-texture-day"
else
    map_texture_asset_id = "tilemap-texture-night"
end

Level = {
    ----------------------------------------------------
    -- Table to define the list of assets
    ----------------------------------------------------
    assets = {
        [0] =
        { type = "texture", id = "tilemap-texture-day",     file = "./assets/tilemaps/jungle.png" },
        { type = "texture", id = "tilemap-texture-night",   file = "./assets/tilemaps/jungle-night.png" },
        { type = "texture", id = "f22-texture",             file = "./assets/images/f22-spritesheet.png" },
        { type = "texture", id = "chopper-texture",         file = "./assets/images/chopper-green-spritesheet.png" },
        { type = "texture", id = "tank-texture",            file = "./assets/images/tank-tiger-right.png" },
        { type = "texture", id = "bullet-texture",          file = "./assets/images/bullet.png" },
        { type = "font"   , id = "pico8-font-5",            file = "./assets/fonts/pico8.ttf", font_size = 5 },
        { type = "font"   , id = "pico8-font-10",           file = "./assets/fonts/pico8.ttf", font_size = 10 },
        { type = "font"   , id = "charriot-font-20",        file = "./assets/fonts/charriot.ttf", font_size = 20 },
        { type = "font"   , id = "charriot-font-9",         file = "./assets/fonts/charriot.ttf", font_size = 9 }

    },

    ----------------------------------------------------
    -- table to define the map config variables
    ----------------------------------------------------
    tilemap = {
        map_file = "./assets/tilemaps/jungle.map",
        texture_asset_id = map_texture_asset_id,
        num_rows = 20,
        num_cols = 25,
        tile_size = 32,
        scale = 2.0
    },

    ----------------------------------------------------
    -- table to define entities and their components
    ----------------------------------------------------
    entities = {
        [0] =
        {
            -- Player
            tag = "player",
            components = {
                transform = {
                    position = { x = 242, y = 110 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "chopper-texture",
                    width = 32,
                    height = 32,
                    z_index = 4,
                    fixed = false,
                    src_rect_x = 0,
                    src_rect_y = 0
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    width = 32,
                    height = 25,
                    offset = { x = 0, y = 5 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 200, y = 200 },
                    projectile_duration = 10, -- seconds
                    repeat_frequency = 0, -- seconds
                    hit_percentage_damage = 10,
                    friendly = true
                },
                keyboard_controller = {
                    up_velocity = { x = 0, y = -50 },
                    right_velocity = { x = 50, y = 0 },
                    down_velocity = { x = 0, y = 50 },
                    left_velocity = { x = -50, y = 0 }
                },
                camera_follow = {
                    follow = true
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 200, y = 497 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                boxcollider = {
                    width = 25,
                    height = 18,
                    offset = { x = 0, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 100, y = 0 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 20,
                    friendly = false
                }
            }
        },
                {
            -- F-22 fighter jet
            group = "enemies",
            components = {
                transform = {
                    position = { x = 10, y = 10 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 90.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "f22-texture",
                    width = 32,
                    height = 32,
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    width = 32,
                    height = 32
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 200, y = 0 },
                    projectile_duration = 1, -- secondsm
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                },
                on_update_script = {
                    [0] =
                    function(entity, delta_time, ellapsed_time)
                        print("Executing F-22 Lua script!")

                        --[[
                        -- change the position of the the airplane to follow a sine wave movement
                        local new_x = ellapsed_time * 0.09
                        local new_y = 200 + (math.sin(ellapsed_time * 0.001) * 50)
                        set_position(entity, new_x, new_y) -- set the new position
                        --]]
                    end
                }
            }
        }
    }
}

-- Define some useful global variables
map_width = Level.tilemap.num_cols * Level.tilemap.tile_size * Level.tilemap.scale
map_height = Level.tilemap.num_rows * Level.tilemap.tile_size * Level.tilemap.scale