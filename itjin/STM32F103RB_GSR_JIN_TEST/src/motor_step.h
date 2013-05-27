
#ifndef  MOTOR_STEP_PRESENT
#define  MOTOR_STEP_PRESENT

#define STEP_MOTOR_PORT         GPIOB
#define STEP_MOTOR_ENABLE       GPIO_Pin_10
#define STEP_MOTOR_HALF_FULL    GPIO_Pin_11
#define STEP_MOTOR_LEFT_DIR     GPIO_Pin_12
#define STEP_MOTOR_LEFT_PULSE   GPIO_Pin_13
#define STEP_MOTOR_RIGHT_DIR    GPIO_Pin_14
#define STEP_MOTOR_RIGHT_PULSE  GPIO_Pin_15

typedef enum
{
	STEP_MOTOR_Start,
	STEP_MOTOR_Stop,
	STEP_MOTOR_MAX
} step_motor_act_type;

// Motor direction type
typedef enum
{
    MOTOR_DIR_CCW =0,
    MOTOR_DIR_CW,
    MOTOR_DIR_MAX
}step_motor_dir_type;

typedef enum
{
    ROBOT_GO_FORWARD,
    ROBOT_GO_BACKWARD,
    ROBOT_TURN_LEFT,
    ROBOT_TURN_RIGHT,
    ROBOT_GO_MAX
}step_motor_robot_dir_type;


extern u16 g_step_motor_count;
extern u16 g_isr_step_motor_10ms_cnt;



extern void isr_motor_event(void);
extern void step_motor_direction(step_motor_dir_type l_motor_dir, step_motor_dir_type r_motor_dir);
extern void step_motor_enable(FunctionalState chip_sel);
extern void step_motor_start(step_motor_robot_dir_type robot_direction);
extern void step_motor_stop(void);

// Service menu
extern void service_step_motor_start(void);
extern void service_step_motor_stop(void);
extern void service_step_motor_cruise_test(void);
extern void service_step_motor_exit(void);

#endif /* MOTOR_STEP_PRESENT */