#include "klone_conf.h"
#include <string.h>
#include <klone/field.h>
#include <klone/utils.h>
#include <u/libu.h>

/** 
 *  \ingroup Chttp
 *  \{
 */

/**
 *          \defgroup field HTTP header fields handling
 *          \{
 *              \par
 */

/**
 * \brief   Set a field
 *
 * Set field \a f to have \a name and \a value.
 *  
 * \param f     field object
 * \param name  field name
 * \param value field value
 *      
 * \return
 *  - \c 0  if successful
 *  - \c ~0 on error
 */     
int field_set(field_t* f, const char *name, const char *value)
{
    char *n = NULL, *v = NULL;

    n = u_strdup(name);
    dbg_err_if(n == NULL);

    v = u_strdup(value);
    dbg_err_if(v == NULL);

    if(f->name)
        u_free(f->name);

    if(f->value)
        u_free(f->value);

    f->name = n;
    f->value = v;

    return 0;
err:
    if(n)
        u_free(n);
    if(v)
        u_free(v);
    return ~0;
}

/**
 * \brief   Set a field from a line
 *
 * Set the name and value of field \a f. Name and value must be separated by 
 * \e ":".
 * 
 * \param f     field object
 * \param ln    line
 *  
 * \return
 *  - \c 0  if successful
 *  - \c ~0 on error
 */ 
int field_set_from_line(field_t* f, const char *ln)
{
    enum { BUFSZ = 256 };
    char *p, *name = NULL;

    dbg_err_if(!ln || !strlen(ln));

    dbg_err_if((p = strchr(ln, ':')) == NULL);

    name = u_strndup(ln, p-ln);
    dbg_err_if(name == NULL);

    /* eat blanks between ':' and value */
    for(++p; u_isblank(*p); ++p)
        ;

    dbg_err_if(field_set(f, name, p));

    if(name)
        u_free(name);

    return 0;
err:
    if(name)
        u_free(name);
    return ~0;
}

/** 
 * \brief   Get the name of a field
 *  
 * Return the string value of field \a f.
 *
 * \param f field object
 *  
 * \return
 *  - the string corresponding to the field name (null-terminated)
 */
const char* field_get_name(field_t *f)
{
    return f->name; /* may be null */
}

/** 
 * \brief   Get the value of a field
 *  
 * Return the string value of field \a f.
 *
 * \param f field object
 *  
 * \return
 *  - the string corresponding to the field value (null-terminated)
 */
const char* field_get_value(field_t *f)
{
    return f->value; /* may be null */
}

/** 
 * \brief   Create a field
 *  
 * Create a field from \a name and \a value into \a pf.
 *
 * \param name  field name
 * \param value field value
 * \param pf    address of field pointer
 *  
 * \return
 *  - \c 0  if successful
 *  - \c ~0 on error
 */
int field_create(const char *name, const char *value, field_t **pf)
{
    field_t *f = NULL;

    f = u_zalloc(sizeof(field_t));
    dbg_err_if(f == NULL);

    if(name)
        dbg_err_if((f->name = u_strdup(name)) == NULL);

    if(value)
        dbg_err_if((f->value = u_strdup(value)) == NULL);

    *pf = f;

    return 0;
err:
    if(f)
        field_free(f);
    return ~0;
}

/** 
 * \brief   Free a field
 *  
 * Free field \a f.
 *
 * \param f field object
 *  
 * \return
 *  - \c 0  always
 */
int field_free(field_t *f)
{
    if(f)
    {
        if(f->name)
            u_free(f->name);
        if(f->value)
            u_free(f->value);
        u_free(f);
    }

    return 0;
}

/**
 *          \}
 */

/**
 *  \}
 */
