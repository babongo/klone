#include <klone/emb.h>
#include <klone/debug.h>

/* these are klone-site autogen functions */
int register_pages();
int unregister_pages();

static struct emblist_s list;   /* list of emb resources     */
static size_t nres;             /* num of emb resources      */
static int init = 0;

int emb_init()
{
    if(init++ == 0)
    {
        LIST_INIT(&list); /* init resource list */

        /* call autogen external function (cannot be called more then once!) */
        dbg("registering embedded resources");
        register_pages();
    }

    return 0;
}

int emb_term()
{
    dbg_err_if(init == 0);

    dbg("unregistering embedded resources");
    unregister_pages();

    return 0;
err:
    return ~0;
}

int emb_register(embres_t *res)
{
    dbg_err_if(init == 0 || res == NULL);

    if(res->type == ET_FILE) 
        dbg("registering %s (%s)", res->filename, 
                ((embfile_t*)res)->comp ? "compressed" : "uncompressed");
    else 
        dbg("registering %s", res->filename);

    LIST_INSERT_HEAD(&list, res, np);
    nres++;

    return 0;
err:
    return ~0;
}

int emb_unregister(embres_t *res)
{
    dbg_err_if(init == 0 || res == NULL);

    LIST_REMOVE(res, np);
    nres--;

    return 0;
err:
    return ~0;
}

int emb_lookup(const char *filename, embres_t **pr)
{
    embres_t *res;

    dbg_err_if(init == 0 || filename == NULL || pr == NULL ||!strlen(filename));

    LIST_FOREACH(res, &list, np)
    {
        if(strcmp(filename, res->filename))
            continue;

        /* save found resource pointer */
        *pr = res;

        return 0; /* found */
    }

err:
    /* not found */
    return ~0;
}

int emb_count()
{
    dbg_err_if(init == 0);

    return nres;
err:
    return -1;
}

int emb_getn(size_t n, embres_t **pr)
{
    embres_t *res = NULL;

    dbg_err_if(init == 0 || n >= nres);

    LIST_FOREACH(res, &list, np)
    {
        if(n-- == 0)
            break;
    }

    *pr = res;

    return 0;
err:
    return ~0;
}


