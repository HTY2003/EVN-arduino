``EVNMotor``
============

EVNMotor uses timer-generated interrupts to achieve consistent speed and position control. Using these functions, we have replicated most of the motor functions EV3 users will be familiar with.
We have built-in motor profiles for NXT Large Motors, EV3 Medium Motors and EV3 Large Motors, but the user may edit them if they wish to.

.. note:: EVNMotor consumers some of the RP2040's spinlock peripherals. See this `page`_ for more info.

.. _page: ../getting-started/hardware-overview.html

.. note:: For gearmotors plugged into the 6-pin headers, users may have to set default parameters for the CUSTOM_MOTOR preset in `src/evn_motor_pids.h`, such as maximum RPM and encoder PPR (pulses per revolution).

.. warning:: A gearmotor and a LEGO motor should not be simultaneously plugged into the same motor port, as the 6-pin headers are wired in parallel with the LEGO ports.

Constructor
-----------

.. class:: EVNMotor(uint8_t port, uint8_t motor_type = EV3_LARGE, uint8_t motor_dir = DIRECT, uint8_t enc_dir = DIRECT)
    
    :param port: Port the motor is connected to (1 - 4)
    
    :param motor_type: Type of motor connected. Defaults to ``EV3_LARGE``

        * ``EV3_LARGE`` - EV3 Large Servo Motor
        * ``EV3_MED`` - EV3 Medium Servo Motor
        * ``NXT_LARGE`` - NXT Large Servo Motor
        * ``CUSTOM_MOTOR`` - Custom Gearmotor
    
    :param motor_dir: When set to ``REVERSE``, flips motor **and** encoder pins, inverting the direction of the motor. Defaults to ``DIRECT``

        * ``DIRECT`` - Do not flip direction
        * ``REVERSE`` - Flip direction

    :param enc_dir: When set to ``REVERSE``, flips encoder pins **only**. Not necessary for LEGO motors, but useful for non-LEGO gearmotors when the encoder input and motor output increment in opposing directions. Defaults to ``DIRECT``

    .. code-block:: cpp

        EVNMotor motor1(1, EV3_MED);
        EVNMotor motor2(2, EV3_LARGE);
        EVNMotor motor3(3, NXT_LARGE, REVERSE);
        EVNMotor motor4(4, CUSTOM_MOTOR, DIRECT, REVERSE);

Functions
---------

.. function:: void begin()

    Initializes motor object. Call this function before calling the other EVNMotor functions (including settings functions).

    .. code-block:: cpp

        EVNMotor motor(1, EV3_MED);

        void setup1()   //call on setup1(), not setup()!
        {
            motor.begin();
        }

.. note::
    This command should be run on the 2nd core using ``void setup1()``. 
    However, you can still call the movement and settings functions in ``void loop()`` like a normal program.

Measurements
""""""""""""

.. function:: float getPosition()

    :returns: Angular displacement of motor from its starting position, in degrees

    .. code-block:: cpp

        float pos = motor.getPosition();

.. function:: float getHeading()

    :returns: Motor position converted to range from 0-360 degrees

    .. code-block:: cpp

        float pos = motor.getHeading(); //ranges from 0 to 360

.. function:: void setPosition(float position)

    Reset motor's current position to given position

    :param position: Position (in degrees) for motor position to be set to

    .. code-block:: cpp

        motor.setPosition(60);
        //afterwards, getPosition will return 60

.. function:: void resetPosition()

    Reset motor's current position to 0

    .. code-block:: cpp

        motor.resetPosition();
        //afterwards, getPosition will return 0

.. function::   float getSpeed()

    :returns: Angular velocity of motor, in DPS (degrees per second)

    .. code-block:: cpp

        float speed = motor.getSpeed();

.. function:: bool stalled()

    :returns: Boolean indicating when motor is stalled (unable to reach target velocity)

    .. code-block:: cpp

        bool is_motor_stalled = motor.stalled();

.. function:: float getMaxRPM()

    :returns: Maximum speed in RPM (revolutions per minute) of motor
    
.. function:: float getMaxDPS()

    :returns: Maximum speed in DPS (degrees per second) of motor, which is max RPM * 6

Run Forever
"""""""""""

.. function:: void runPWM(float duty_cycle_pct)

    Runs the motor at the given duty cycle (in %) using PWM until a new command is called. Motor speed will vary with load torque applied.

    :param duty_cycle_pct: duty cycle to run the motor at in % (number from -100 to 100)

    .. code-block:: cpp

        //run motor at 80% duty cycle
        motor.runPWM(80);

.. function::   void runSpeed(float dps)

    Runs the motor at the given angular velocity until a new command is called. Motor will attempt to maintain constant speed despite varying load torque.

    :param dps: Angular velocity to run the motor at (in DPS)

    .. code-block:: cpp

        //run motor at 300DPS in the negative direction
        motor.runSpeed(-300);

Run by a Fixed Amount
"""""""""""""""""""""
.. function:: void runAngle(float dps, float degrees, uint8_t stop_action = STOP_BRAKE, bool wait = true)

    Run motor by the given angle (relative to its starting position), then performs the given stop action.

    Motor direction is reversed when the ``degrees`` or ``dps`` inputs are negative (e.g. ``runAngle(100, -100)``, ``runAngle(-100, 100)``, or ``runAngle(-100, -100)`` will all run the motor in reverse).

    :param dps: Angular velocity to run the motor at (in DPS)
    :param degrees: Angular displacement which the motor has to travel (in degrees)
    :param stop_action: Behaviour of the motor upon completing its command. Defaults to ``STOP_BRAKE``

        * ``STOP_BRAKE`` -- Brake (Slow decay)
        * ``STOP_COAST`` -- Coast (Fast decay)
        * ``STOP_HOLD`` -- Hold position

    :param wait: Block function from returning until command is finished

    .. code-block:: cpp

        //run motor for 360 degrees of rotation at speed 300DPS
        motor.runAngle(300, 360, STOP_BRAKE);

.. function:: void runPosition(float dps, float position, uint8_t stop_action = STOP_BRAKE, bool wait = true)

    Run motor to the given motor shaft position, then performs the given stop action.

    :param dps: Angular velocity to run the motor at (in DPS)
    :param position: Position which the motor has to travel to (in degrees)
    :param stop_action: Behaviour of the motor upon completing its command. Defaults to ``STOP_BRAKE``

        * ``STOP_BRAKE`` -- Brake (Slow decay)
        * ``STOP_COAST`` -- Coast (Fast decay)
        * ``STOP_HOLD`` -- Hold position
    
    :param wait: Block function from returning until command is finished

    .. code-block:: cpp

        //return motor to position 0 at speed 300DPS
        motor.runPosition(300, 0, STOP_BRAKE);

.. function:: void runHeading(float dps, float heading, uint8_t stop_action = STOP_BRAKE, bool wait = true)

    Run motor to the specified motor shaft heading, then performs the given stop action.

    Motor direction will be automatically computed to minimize the rotation needed to achieve the correct heading.

    :param dps: Angular velocity to run the motor at (in DPS)
    :param time_ms: Heading which the motor has to travel to (0 - 360 degrees)
    :param stop_action: Behaviour of the motor upon completing its command. Defaults to ``STOP_BRAKE``

        * ``STOP_BRAKE`` -- Brake (Slow decay)
        * ``STOP_COAST`` -- Coast (Fast decay)
        * ``STOP_HOLD`` -- Hold position

    :param wait: Block function from returning until command is finished

    .. code-block:: cpp

        //return motor to heading 0 at speed 300DPS (i.e. position % 360 = o)
        motor.runHeading(300, 0, STOP_BRAKE);

.. function:: void runTime(float dps, uint32_t time_ms, uint8_t stop_action = STOP_BRAKE, bool wait = true)

    Run motor for the given amount of time, then performs the given stop action.

    :param dps: Angular velocity to run the motor at (in DPS)
    :param time_ms: Time which the motor has to run for (in milliseconds)
    :param stop_action: Behaviour of the motor upon completing its command. Defaults to ``STOP_BRAKE``

        * ``STOP_BRAKE`` -- Brake (Slow decay)
        * ``STOP_COAST`` -- Coast (Fast decay)
        * ``STOP_HOLD`` -- Hold position

    :param wait: Block function from returning until command is finished

    .. code-block:: cpp

        //run motor for 3 seconds at speed 300DPS
        motor.runTime(300, 3000, STOP_BRAKE);

.. function:: bool completed()

    :returns: Boolean indicating whether the motor has hit its target position / completed running for the set amount of time

    .. code-block:: cpp

        //ensure that motor has completed command before proceeding
        while (!motor.completed());

Stopping
"""""""""

.. function:: void stop()

    Brakes the motor (slow decay).

    .. code-block:: cpp
        
        motor.stop();

.. function:: void coast()

    Coasts the motor (fast decay).
    Compared to braking with `stop()`, motor comes to a stop more slowly.

    .. code-block:: cpp
        
        motor.coast();

.. function:: void hold()

    Hold the motor in its current position. Restricts the motor shaft from moving.

    .. code-block:: cpp
        
        motor.hold();

Control Settings
""""""""""""""""

To view the default PD and accel/decel values, look at ``src\evn_motor_defs.h`` in the Github repository.

.. function:: void setKp(float kp)

    Sets proportional gain values for the PD controller (controls rotational/angular velocity of motor shaft).

    The error for the controller is the difference between the robot's target amount of rotations (which increases over time) and the angle the robot has currently rotated by.

    :param kp: Proportional gain

    .. code-block:: cpp

        motor.setKp(0.2);

.. function:: void setKd(float kd)

    Sets derivative gain values for the PD controller (controls rotational/angular velocity of motor shaft).

    The error for the controller is the difference between the robot's target amount of rotations (which increases over time) and the angle the robot has currently rotated by.

    :param kd: Derivative gain

    .. code-block:: cpp

        motor.setKd(0.00600);

.. function:: void setAccel(float accel_dps_per_s)

    Set acceleration value of motor (in deg/s^2). This value can be adjusted to avoid wheel slippage caused by high accelerations.

    :param accel_dps_per_s: Acceleration in DPS per second

    .. code-block:: cpp

        motor.setAccel(500);

.. function:: void setDecel(float decel_dps_per_s)

    Set deceleration value of motor (in deg/s^2). This value can be adjusted to avoid wheel slippage caused by high accelerations.

    :param accel_dps_per_s: Deceleration in DPS per second

    .. code-block:: cpp

        motor.setDecel(500);

.. function:: void setMaxRPM(float max_rpm)

    Set max RPM (revolutions per minute) of motor

    :param max_rpm: Maximum RPM of motor

    .. code-block:: cpp

        motor.setMaxRPM(140);

.. function:: void setPPR(float ppr)

    Set pulses per revolution of motor shaft. For all LEGO EV3/NXT motors, PPR is 360 so it requires no adjustment.

    Some motor manufacturers specify the motor's CPR (counts per revolution), which is 4 times of a motor's PPR.

    :param ppr: Pulses per revolution of motor

    .. code-block:: cpp

        motor.setPPR(823);

.. function:: void setDebug(bool enable)

    Used to toggle debug mode, where motor will print the error used for PD control over ``Serial``. Can be used to observe or tune PD controller behaviour.

    :param enable: Whether to enable debug mode

How Our Motor Control Works
""""""""""""""""""""""""""""
This is a little technical, but feel free to skip it!

For move functions where the motor rotates by a fixed amount, what we actually do is set a **target position** for the motor to move to. 
This target position starts out as the motor's **current position**, but **increments over time** until it reaches the **end position** given by the user.

It increments at the speed given by the user, so if the user wants to run their motor at 30DPS, the position signal increases at a rate of 30 degrees per second.

Now that we have the position signal, we need a way to command our motor to follow this position signal closely (thus moving at the desired speed to the desired endpoint).
We use a **Proportional-Derivative (PD) controller** to do so. It receives the **error** between our motor's current position and the target position signal, and **outputs the required duty cycle** we need to run our motor at.

However, this approach usually requires one to **tune** the PD controller's settings to ensure the motor follows the position signal closely, without being too slow or oscillating. 
Tuning motor PIDs is a bit tricky (you won't have to do it for LEGO motors), but we will be creating a guide for it soon!