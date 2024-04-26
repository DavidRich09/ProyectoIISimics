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



/* This defines a new interface type. Its corresponding C data type will be
   called "control_Device_PCI_interface_t". */
SIM_INTERFACE(control_Device_PCI) {
    void (*start_operation)(conf_object_t *obj);
    void (*clear_done)(conf_object_t *obj);
};

/* Use a #define like this whenever you need to use the name of the interface
   type; the C compiler will then catch any typos at compile-time. */
#define CONTROL_DEVICE_PCI_INTERFACE "control_Device_PCI"

#ifdef __cplusplus
}
#endif

#endif /* ! CONTROL_DEVICE_PCI_INTERFACE_H */
