#ifndef ADNS2610_DRV_H
#define ADNS2610_DRV_H

#include "OptiMouse.h"

#if OPTI_MOUSE_PROC_RAW_IMAGE == 1

#define RAW_IMAGE_SIZE 			18 		/* size of sensor side in pixels */
#define _RAW_IMAGE_DATA_OFFSET	0x00	/* raw data register offset in bytes */
#define _RAW_IMAGE_MODE_REG		0x00	/* optical mouse sensor mode register address */    
#define _RAW_IMAGE_MODE_ON		0x01	/* optical mouse sensor mode on value */
#define _RAW_IMAGE_MODE_OFF		0x00	/* optical mouse sensor mode off value */

#define opti_mouse__raw_image_mode_on() opti_mouse__write_register(_RAW_IMAGE_MODE_REG, _RAW_IMAGE_MODE_ON); 
#define opti_mouse__raw_image_mode_off() opti_mouse__write_register(_RAW_IMAGE_MODE_REG, _RAW_IMAGE_MODE_OFF); 

#endif

#endif /* ADNS2610_DRV_H */
