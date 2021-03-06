#include <stdlib.h>

#include "eval.h"
#include "scheme-data-types.h"
#include "utils.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-or.h"

/**** Private variables ****/

static scheme_procedure _procedure_or;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "or".
 *
 * Will return NULL if:
 * - Element is not a list in the format: (<element> ...).
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Result of evaluation or NULL if an error occurs.
 */
static scheme_element *_or_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_or_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    // Evaluate each argument.
    scheme_pair *evaluatedList = scheme_list_evaluated((scheme_pair *)element, namespace);
    if (evaluatedList == NULL)
    {
        return NULL;
    }

    // Get arguments.
    int argCount;
    scheme_element **arguments = scheme_list_to_array(evaluatedList, &argCount);

    // Terminate if element is not a list.
    if (argCount == -1)
    {
        return NULL;
    }

    // If there are no arguments, simply return #f.
    if (argCount == 0)
    {
        return (scheme_element *)scheme_boolean_get_false();
    }

    // Go through each evaluated argument and return one as soon as we find one that
    // is not null (ie. not equal to #f).
    for (int i = 0; i < argCount; ++i)
    {
        if (!scheme_element_compare(arguments[i], (scheme_element *)scheme_boolean_get_false()))
        {
            scheme_element *result = scheme_element_copy(arguments[i]);
            free(arguments);
            scheme_element_free((scheme_element *)evaluatedList);
            return result;
        }
    }

    // No argument fonud. Return evaluated last argument.
    scheme_element *last = scheme_element_copy(arguments[argCount - 1]);
    free(arguments);
    scheme_element_free((scheme_element *)evaluatedList);
    return last;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_get()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_or, PROCEDURE_OR_NAME, _or_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_or.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_or.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_or;
}

