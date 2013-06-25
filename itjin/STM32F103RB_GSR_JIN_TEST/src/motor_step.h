
#ifndef  MOTOR_STEP_PRESENT
#define  MOTOR_STEP_PRESENT

//1 You MUST set below definitions in accordance with your robot.

#define STEP_MOTOR_Pulse_Low_Delay      100     // 100us
#define STEP_MOTOR_One_Step_dist_mm     0.24    // Half step: 0.9 degree, Radius of a wheel : 15mm
                                                // 2 x 3.141(pi) x R = 2 x 3.141 x 15 = 94.23 mm
                                                // 1 degree distance = 94.23mm / 360(degree) = 0.261mm
                                                // 0.9 degree = (0.261 / 10) x 9 = 0.2349 => 0.24mm
#define ROBOT_Max_Speed                 500     // Max speed table
#define ROBOT_Turn_Speed                50      // Robot turn speed max table
#define ROBOT_Turn_Left_Step_For_90_Degree      200 // Step count for turn left
#define ROBOT_Turn_Right_Step_For_90_Degree     200 // Step count for turn right
#define ROBOT_Smooth_Turn_Dec_Table             400 // Decrease motor table from max speed table
#define ROBOT_Smooth_Turn_Const_Dist            900 // Constant distant in low step table

#define STEP_MOTOR_PORT         GPIOB
#define STEP_MOTOR_ENABLE       GPIO_Pin_10
#define STEP_MOTOR_HALF_FULL    GPIO_Pin_11
#define STEP_MOTOR_LEFT_DIR     GPIO_Pin_12
#define STEP_MOTOR_LEFT_PULSE   GPIO_Pin_13
#define STEP_MOTOR_RIGHT_DIR    GPIO_Pin_14
#define STEP_MOTOR_RIGHT_PULSE  GPIO_Pin_15
//1 Until this line --------------

#define MM_TO_STEP(x) ((u16)( x / (float)STEP_MOTOR_One_Step_dist_mm))


// Motor direction type
typedef enum
{
    STEP_MOTOR_DIR_CCW,
    STEP_MOTOR_DIR_CW,
    STEP_MOTOR_DIR_MAX
}step_motor_dir_type;

// Motor step angle
typedef enum
{
    STEP_MOTOR_ANGLE_FULL,          // 1.8
    STEP_MOTOR_ANGLE_HALF,          // 0.9
    STEP_MOTOR_ANGLE_MAX
}step_motor_angle_type;

typedef enum
{
    ACT_GO_FORWARD,
    ACT_GO_BACKWARD,
    ACT_TURN_LEFT,
    ACT_TURN_RIGHT,
    ACT_MAX
}robot_action_type;

typedef enum
{
    ADJUSMENT_FRONT,                // Adjusment during forward movement
    ADJUSMENT_BACK,                 // Adjusment during backward movement
    NOT_ADJUSMENT,                  // Just move fixed distance
    SETTING_ONLY,
}robot_with_action_type;

typedef enum
{
    TURN_LEFT,
    TURN_RIGHT,
    TURN_MAX
}robot_turn_type;

typedef enum
{
    TURN_45_DEGREE = 45,
    TURN_90_DEGREE = 90,
    TURN_180_DEGREE = 180,    
    TURN_270_DEGREE = 270,    
    TURN_360_DEGREE = 360,        
}robot_turn_degree_type;

typedef enum
{
    STOP_EMERGENCY,
    STOP_NORMAL,
    STOP_MAX
}robot_stop_type;


typedef struct
{
    step_motor_dir_type dir;
    step_motor_angle_type angle;
    u16 step_count;                 // Real motor step count
    u16 max_speed;                  // Robot speed. max motor table count
    u16 target_distance;            // Robot movement step
    u16 constant_distance;          // Robot constant velocity step
    u16 adjustment_time;            // The time for Robot adjusment.
    // smooth turn
    FunctionalState  smooth_turn_flag;
    u16 smooth_speed_down_max_table;
    u16 smooth_constant_dist;
    u16 smooth_target_dist;
    u16 smooth_step_count;
}step_motor_set_type;


extern step_motor_set_type g_left_mot, g_right_mot;


extern void isr_left_motor_event(void);
extern void isr_right_motor_event(void);

extern void step_motor_direction(step_motor_dir_type l_motor_dir, step_motor_dir_type r_motor_dir);
extern void step_motor_enable(FunctionalState chip_sel);
extern void step_motor_angle(step_motor_angle_type angle);
extern void motor_all(u16 movement_millimeter, robot_action_type robot_action, robot_with_action_type with_what);
extern void motor_stop(robot_stop_type stop);
extern void step_motor_toque_off(void);
extern void robot_adjusment(robot_with_action_type adj_position);
extern void robot_adj_line_sensor(robot_with_action_type adj_position, u16 *l_adj_time, u16 *r_adj_time);
extern void robot_turn(robot_turn_type act, u16 turn_degree);
extern void test_func_robot_turn(robot_turn_type act, u16 turn_step);
extern void test_func_robot_smooth_turn(robot_turn_type smooth_act);

// Service menu
extern void service_step_motor_start(void);
extern void service_step_motor_stop(void);
extern void service_step_motor_cruise_test(void);
extern void service_step_motor_exit(void);

#endif /* MOTOR_STEP_PRESENT */
