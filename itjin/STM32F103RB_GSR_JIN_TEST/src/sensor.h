#ifndef  SENSOR_PRESENT
#define  SENSOR_PRESENT

typedef enum
{
    SENSOR_FRONT_LINE,
    SENSOR_BACK_LINE,
    SENSOR_TOP_AD,
    SENSOR_BOTTOM_AD,
    SENSOR_LEFT_AD,
    SENSOR_RIGHT_AD,
    SENSOR_MAX   
}robot_sensor_type;

extern u16 robot_read_sensor(robot_sensor_type sensor);


#endif /* SENSOR_PRESENT */

