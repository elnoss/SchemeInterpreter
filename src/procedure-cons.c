#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "scheme-pair-utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-cons.h"

/**** Private variables ****/

static scheme_procedure _procedure_cons;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "cons".
 * Return a Scheme pair with first and second elements set to the given
 * arguments.
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format: (<element> <element>)
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Constructed pair, or NULL if an error occurred.
 */
static scheme_element *_cons_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_cons_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Get arguments.
    int argCount;
    scheme_element **args = scheme_list_to_array((scheme_pair *)element, &argCount);

    // Check if argument list is invalid.
    if (argCount == -1) return NULL;
    else if (argCount != 2)
    {
        // Wrong number of arguments.
        if (args != NULL) free(args);
        return NULL;
    }

    scheme_element *firstArg = *args;
    scheme_element *secondArg = *(args+1);
    free(args);

    // Evaluate first and second arguments.
    firstArg = scheme_evaluate(firstArg, namespace);
    secondArg = scheme_evaluate(secondArg, namespace);
    if (firstArg == NULL || secondArg == NULL)
    {
        scheme_element_free(firstArg);
        scheme_element_free(secondArg);
        return NULL;
    }

    scheme_pair *pair = scheme_pair_new(firstArg, secondArg);

    scheme_element_free(firstArg);
    scheme_element_free(secondArg);
    return (scheme_element *)pair;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_cons()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_cons, PROCEDURE_CONS_NAME, _cons_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_cons.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_cons.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_cons;
}
