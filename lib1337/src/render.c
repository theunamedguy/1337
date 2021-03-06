#include "1337_imp.h"

void l_render(struct world_t *world)
{
    void *userdata = world->userdata;
    const struct interface_t *interface = world->interface;

    interface->draw_clear(userdata);

    struct camera_t *camera = &world->camera;
    const struct coords_t cam_offs = { world->camera.offset.x, world->camera.offset.y };

    for(llong x = camera->pos.x + camera->size.x, i = camera->size.x; x >= camera->pos.x - 1; --x, --i)
        for(llong y = camera->pos.y - 1, j = camera->size.y; y <= camera->pos.y + camera->size.y; ++y, --j)
        {
            sprite_t sprite, back;
            struct tile_t *tile = l_gettile(world, x, y);
            if(tile)
            {
                sprite = tile->sprite;
                back = tile->background;
                llong offs_x = tile->offset.x, offs_y = tile->offset.y;
                //if(x == player->pos.x && y == player->pos.y)
                //{
                //    sprite = player->sprite;
                //    offs_x = player->offset.x;
                //    offs_y = player->offset.y;
                //}

                interface->draw_sprite(userdata,
                                       i * 32 - cam_offs.x,
                                       j * 32 + cam_offs.y,
                                       back);
                if(sprite != back && !offs_x && !offs_y)
                    interface->draw_sprite(userdata,
                                           i * 32 + offs_x - cam_offs.x,
                                           j * 32 + offs_y + cam_offs.y,
                                           sprite);
            }
        }

    /* draw overlay tiles */
    /* for each block in the blocklist, try to render the overlay tiles */
    /* TODO: optimize */
    struct l33t_data *data = ((struct l33t_data*)world->privatedata);

    struct block_t *block = data->blocks;
    while(block)
    {
        llong b_rx = 32 * (block->coords.x - camera->pos.x) - cam_offs.x;
        llong b_ry = 32 * (camera->size.y - block->coords.y + camera->pos.y - 1) + cam_offs.y;
        struct overlaytile_t *iter = block->overlay;
        while(iter)
        {
            llong rx = b_rx + 32 * iter->_coords.x + iter->offset.x;
            llong ry = b_ry - 32 * iter->_coords.y - iter->offset.y;
            interface->draw_sprite(userdata,
                                   rx, ry,
                                   iter->sprite);
            iter = iter->next;
        }
        block = block->next;
    }

    interface->draw_text(userdata, 0, 0, "pos: (%lld, %lld) + (%lld, %lld)",
                         camera->pos.x, camera->pos.y,
                         camera->offset.x, camera->offset.y);
    interface->draw_text(userdata, 0, 32, "block: (%d, %d)",
                         ROUND_BLOCK(camera->pos.x),
                         ROUND_BLOCK(camera->pos.y));
    interface->draw_text(userdata, 0, 64, "blocklen: %d",
                         ((struct l33t_data*)(world->privatedata))->blocklen);
    interface->draw_update(userdata);
}
