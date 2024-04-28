/*
  © 2017 Intel Corporation
*/

/* 	 
  control_Device_PCI-interface.h - Skeleton code to base new interface modules on
*/
	 
/* This module is a template for defining new interface types. See the
   "Defining New Interface Types" section of the
   "Simics Model Builder User's Guide" for further documentation.

   The corresponding DML definition can be found in control_Device_PCI_interface.dml */

#ifndef CONTROL_DEVICE_PCI_INTERFACE_H
#define CONTROL_DEVICE_PCI_INTERFACE_H

#include <simics/device-api.h>
#include <simics/pywrap.h>

#ifdef __cplusplus
extern "C" {
#endif

/* If you need to define new struct types, a definition like this will make it
   possible to allocate such structs from Python using control_Device_PCI_data_t().

   Before doing this, you will have to load the control_Device_PCI_interface
   Simics module, and import the control_Device_PCI_interface Python module. */
typedef struct {
        int level;
        char *name;
} control_Device_PCI_data_t;
SIM_PY_ALLOCATABLE(control_Device_PCI_data_t);

/* This defines a new interface type. Its corresponding C data type will be
   called "control_Device_PCI_interface_t". */
SIM_INTERFACE(control_Device_PCI) {
        bool (*example_method)(conf_object_t *obj, int id,
                               control_Device_PCI_data_t *data);
#ifndef PYWRAP
        /* methods that cannot be exported to Python, for example as it refers
           to unknown data types, must be enclosed by "#ifndef PYWRAP" ...
           "#endif". See the "Restrictions" subsection of "Defining New
           Interface Types" mentioned above. */
        void (*no_python_method)(conf_object_t *obj, size_t bufsize, void *buf);
        void (*start_read)(conf_object_t *obj, uint32 value);
        uint16 (*read_buffer)(conf_object_t *obj, uint16 index);
        void (*move_offeset)(conf_object_t *obj);
        uint32 (*read_file_size)(conf_object_t *obj);

#endif
};

/* Use a #define like this whenever you need to use the name of the interface
   type; the C compiler will then catch any typos at compile-time. */
#define CONTROL_DEVICE_PCI_INTERFACE "control_Device_PCI"

#ifdef __cplusplus
}
#endif

#endif /* ! CONTROL_DEVICE_PCI_INTERFACE_H */
