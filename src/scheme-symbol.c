#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scheme-symbol.h"
#include "scheme-element-private.h"

// Scheme symbol.
struct scheme_symbol {
    struct scheme_element super;
    char *value;
    int length;  // Length of value NOT including \0
};

/**** Private function declarations ****/

/**
 * Initialize static variables.
 */
static void _static_init();

/**
 * Return symbol's type identifier.
 *
 * @return Type identifier.
 */
static char *_vtable_get_type();

/**
 * Free Scheme symbol.
 *
 * @param  element  Should be a Scheme symbol.
 */
static void _vtable_free(scheme_element *element);

/**
 * Print symbol to stdout.
 *
 * @param  element  Should be a Scheme symbol.
 */
static void _vtable_print(scheme_element *element);

/**
 * Copy symbol.
 *
 * @param  element  Should be a Scheme symbol.
 *
 * @return Copy or NULL if out of memory.
 */
static scheme_element *_vtable_copy(scheme_element *element);

/**
 * Compare Scheme symbol to another symbol.
 * Will return 0 if either pointers are not symbols.
 *
 * @param  element  Should be a Scheme symbol.
 * @param  other    A Scheme element.
 *
 * @return 1 if equal, 0 otherwise.
 */
static int _vtable_compare(scheme_element *element, scheme_element *other);

/**** Private variables ****/

// Global virtual function table.
static struct scheme_element_vtable _scheme_symbol_vtable;

static int _static_initialized = 0;

/**** Private function implementations ****/

static void _static_init()
{
    if (!_static_initialized)
    {
        _scheme_symbol_vtable.get_type = _vtable_get_type;
        _scheme_symbol_vtable.free = _vtable_free;
        _scheme_symbol_vtable.print = _vtable_print;
        _scheme_symbol_vtable.copy = _vtable_copy;
        _scheme_symbol_vtable.compare = _vtable_compare;

        _static_initialized = 1;
    }
}

static char *_vtable_get_type()
{
    return SCHEME_SYMBOL_TYPE;
}

void _vtable_free(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_SYMBOL_TYPE))
        return;

    scheme_symbol *symbol = (scheme_symbol *)element;

    free((scheme_symbol *)symbol->value);
    free(symbol);
}

static void _vtable_print(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_SYMBOL_TYPE))
        return;

    scheme_symbol *symbol = (scheme_symbol *)element;
    printf("%s", symbol->value);
}

static scheme_element *_vtable_copy(scheme_element *element)
{
    if (!scheme_element_is_type(element, SCHEME_SYMBOL_TYPE))
        return NULL;

    scheme_symbol *symbol = (scheme_symbol *)element;
    return (scheme_element *)scheme_symbol_new(symbol->value);
}

static int _vtable_compare(scheme_element *element, scheme_element *other)
{
    if (!scheme_element_is_type(element, SCHEME_SYMBOL_TYPE)) return 0;
    if (!scheme_element_is_type(other, SCHEME_SYMBOL_TYPE)) return 0;

    char *firstVal = ((scheme_symbol *)element)->value;
    char *secondVal = ((scheme_symbol *)other)->value;

    return strcmp(firstVal, secondVal) == 0;
}

/**** Public function implementations ****/

scheme_symbol *scheme_symbol_new(char *value)
{
    _static_init();

    // Allocate symbol.
    scheme_symbol *symbol;
    if ((symbol = malloc(sizeof(scheme_symbol))) == NULL)
        return NULL;

    // Set up virtual function table.
    symbol->super.vtable = &_scheme_symbol_vtable;

    // Copy value string.
    char *idBuffer;
    int bufLen = strlen(value) + 1;  // Make space for \0
    if ((idBuffer = malloc(sizeof(char) * bufLen)) == NULL)
    {
        free(symbol);
        return NULL;
    }

    strcpy(idBuffer, value);

    symbol->value = idBuffer;
    symbol->length = bufLen;

    return symbol;
}

char *scheme_symbol_get_value(scheme_symbol *symbol)
{
    // Allocate return buffer.
    char *returnBuf;
    int length = symbol->length + 1;  // Make room for \0
    if ((returnBuf = malloc(sizeof(char) * length)) == NULL)
        return NULL;

    // Copy value.
    char *value = symbol->value;
    int i;
    for (i = 0; i < length && value[i] != '\0'; ++i)
    {
        returnBuf[i] = value[i];
    }
    returnBuf[i] = '\0';

    return returnBuf;
}

int scheme_symbol_value_equals(scheme_symbol *symbol, char *value)
{
    if (symbol == NULL || value == NULL) return 0;

    return strcmp(symbol->value, value) == 0;
}