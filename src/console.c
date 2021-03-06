#include "globals.h"

static uint add_char(SDL_Window *wind, SDL_Renderer *rend, char *buf, size_t buflen, uint bufidx, int key)
{
    if((('a' <= key && key <= 'z') || key == ' ' || key == '-' || ('0' <= key && key <= '9') || key == 0) &&
       bufidx < buflen)
    {
        buf[bufidx++] = (char)key;
        vid_printf(wind, rend, "%c", key);
    }
    if(key == '\b')
    {
        printf("got backspace %d\n", bufidx);
        if(bufidx)
            bufidx = bufidx - 1;
        printf("%d\n", bufidx);
    }
    return bufidx;
}

void console_enter(struct world_t *world, SDL_Window *wind, SDL_Renderer *rend)
{
    char buf[128];
    uint bufidx = 0;
    vid_reset();
    vid_printf(wind, rend, "Entered debug console.\nPress tilde again to exit.\n");
    while(1)
    {
        vid_printf(wind, rend, "> ");
        bufidx = 0;
        memset(buf, 0, sizeof(buf));

        while(1)
        {
            SDL_WaitEvent(NULL);
            SDL_Event ev;
            while(SDL_PollEvent(&ev))
            {
                switch(ev.type)
                {
                case SDL_QUIT:
                    exit(1);
                case SDL_KEYDOWN:
                    switch(ev.key.keysym.sym)
                    {
                    case SDLK_BACKQUOTE:
                        return;
                    case SDLK_RETURN:
                        goto exec_cmd;
                    default:
                        bufidx = add_char(wind, rend, buf, sizeof(buf), bufidx, ev.key.keysym.sym);
                        break;
                    }
                    break;
                case SDL_WINDOWEVENT:
                    switch(ev.window.event)
                    {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        window_width = ev.window.data1;
                        window_height = ev.window.data2;
                        l_resize(world, window_width, window_height);
                        SDL_RenderPresent(rend);
                        break;
                    }
                    break;
                }
            }
            SDL_RenderPresent(rend);
        }
        bufidx = add_char(wind, rend, buf, sizeof(buf), bufidx, '\0');
    exec_cmd:
        vid_printf(wind, rend, "\n");
        char *cmd = strtok(buf, " ");
        if(strcmp(cmd, "teleport") == 0)
        {
            /*
            char *xs = strtok(NULL, " ");
            if(xs)
            {
                llong x = strtoll(xs, NULL, 0);
                char *ys = strtok(NULL, " ");
                if(ys)
                {
                    llong y = strtoll(ys, NULL, 0);
                    world->camera.pos.x = x - 10;
                    world->camera.pos.y = y - 10;
                    world->player.pos.x = x;
                    world->player.pos.y = y;
                    generate_view(world);
                    render(world, rend);
                    vid_printf(wind, rend, "teleport successful\n");
                }
            }
            */
        }
        else if(strcmp(cmd, "purge") == 0)
        {
            l_purgeall(world);
            vid_printf(wind, rend, "all blocks written to disk\n");
            l_gen(world);
        }
        else if(strcmp(cmd, "info") == 0)
        {
            vid_printf(wind, rend, "camera position: (%lld, %lld)\n",
                       world->camera.pos.x, world->camera.pos.y);
        }
        else if(strcmp(cmd, "prefix") == 0)
        {
            /*
            char *dir = strtok(NULL, " ");
            if(dir)
                block_setdir(dir);
            else
                vid_printf(wind, rend, "`prefix` requires an argument\n");
            */
        }
        else if(strcmp(cmd, "quit") == 0)
        {
            exit(0);
        }
        else
        {
            vid_printf(wind, rend, "unknown command: %s %d\n", cmd, bufidx);
        }
    }
}
