``EVNIRDistanceSensor`` - Distance measurements using IR light
===============================================================

This class provides the following features and functionalities for our IR Distance Sensor Standard Peripheral (VL53L0X IC):

    * Time-of-Flight (TOF) Distance Measurements using Infrared Light
        
        * Since this measures distance by timing pulses of infrared light, readings will be unreliable on transparent objects (e.g. glass)
    
    * Non-Blocking & Blocking Read Functions
    * Configurable Timing Budget for Readings
    * Built-in I2C Port Selection and De-selection

.. note:: This class does I2C port selection and de-selection automatically, so users do not need to call ``setPort()`` using their EVNAlpha objects.

.. class:: EVNIRDistanceSensor(uint8_t port, uint8_t timing_budget_ms = 33)
  
    :param port: I2C port the sensor is connected to (1-16)

    :param timing_budget_ms: Timing budget allocated for each reading (in ms). Decreasing can lead to lower latency, at the cost of range. Defaults to 33ms

Functions
---------

.. function:: bool begin()
    
    Initializes distance sensor. Call this function before using the other functions.

    :returns: Boolean indicating whether the sensor was successfully initialized. If ``false`` is returned, all other functions will return 0.

.. function:: void setTimingBudget(uint8_t timing_budget_ms)

    Set timing budget for the sensor's readings. Decreasing can lead to lower latency at the cost of range, & vice versa.

    :param timing_budget_ms: Timing budget allocated for each reading (in ms)


.. function:: uint8_t getTimingBudget()

    Get timing budget for the sensor's readings.

    :returns: Timing budget(in ms)

Reading Distance
""""""""""""""""

.. function:: uint16_t read(bool blocking = true)

    Get distance reading from sensor

    :param blocking: Block function from returning a value until a new reading is obtained. Defaults to ``true``

    :returns: Distance reading (in mm)

