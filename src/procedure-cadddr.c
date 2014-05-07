#include <stdlib.h>

#include "eval.h"
#include "utils.h"

#include "procedure-cdr.h"
#include "procedure-car.h"

#include "scheme-data-types.h"
#include "scheme-procedure-init.h"
#include "scheme-element-private.h"

#include "procedure-cadddr.h"

static scheme_procedure _procedure_cadddr;
static struct scheme_element_vtable _procedure_vtable;
static int _proc_initd = 0;

/**** Private function declarations ****/

/**
 * Implementation of Scheme procedure "cadddr".
 *
 * Will return NULL if:
 * - Supplied element is not a pair in the format: (<pair>)
 * - Argument <pair> is not a list with at least 4 elements.
 * - Out of memory.
 *
 * @param  procedure  Procedure that refers to this function.
 * @param  element    A Scheme element.
 * @param  namespace  Active namespace.
 *
 * @return Result of evaluation or NULL if an error occurs.
 */
static scheme_element *_cadddr_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace);

/**
 * Prevent freeing this statically allocated Scheme procedure.
 * This function does nothing.
 *
 * @param  element  Should be this procedure.
 */
static void _procedure_free(scheme_element *element) {}

/**** Private function implementations ****/

static scheme_element *_cadddr_function(scheme_procedure *procedure, scheme_element *element, scheme_namespace *namespace)
{
    scheme_element *result, *parameter, *temp;

    // Call cdr 3 times.
    parameter = scheme_element_copy(element);
    for (int i = 0; i < 3; ++i)
    {
        // Perform cdr call.
        result = scheme_procedure_apply(scheme_procedure_cdr(), parameter, namespace);
        scheme_element_free(parameter);

        if (result == NULL)
        {
            return NULL;
        }

        // Quote result.
        parameter = (scheme_element *)scheme_element_quote(result);
        scheme_element_free(result);

        // Add quoted result to a list of 1 element.
        temp = (scheme_element *)scheme_pair_new(parameter, (scheme_element *)scheme_pair_get_empty());
        scheme_element_free(parameter);
        parameter = temp;
    }

    // Call car on result.
    result = scheme_procedure_apply(scheme_procedure_car(), parameter, namespace);
    scheme_element_free(parameter);

    return result;
}

/**** Public function implementations ****/

scheme_procedure *scheme_procedure_cadddr()
{
    if (!_proc_initd)
    {
        scheme_procedure_init(&_procedure_cadddr, PROCEDURE_CADDDR_NAME, _cadddr_function);

        scheme_element_vtable_clone(&_procedure_vtable, _procedure_cadddr.super.vtable);
        _procedure_vtable.free = _procedure_free;
        _procedure_cadddr.super.vtable = &_procedure_vtable;

        _proc_initd = 1;
    }

    return &_procedure_cadddr;
}
