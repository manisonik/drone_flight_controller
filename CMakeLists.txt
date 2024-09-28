cmake_minimum_required(VERSION 3.22)

#
# User is free to modify the file as much as necessary
#

#list(APPEND CMAKE_MODULE_PATH "C:/Users/user1/Downloads/CmakeTestProject/cmake")
#list(APPEND CMAKE_MODULE_PATH "{{sr:cmake_path}}")
#message("Build CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
include("cmake/gcc-arm-none-eabi.cmake")
#message("Build CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})

# Core project settings
project(Drone-STM32F756)
enable_language(C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Preprocessors
add_compile_definitions(STM32F756xx)

# Core MCU flags, CPU type, instruction set and FPU setup
set(cpu_PARAMS 
    # Other parameters
    # -mthumb
    # -mcpu, -mfloat, -mfloat-abi, ...
)

# Set the directory for the SmartMotion Driver
set(INVN_EMD_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/eMD-SmartMotion-ICM20948-1.1.1/EMD-Core)

# Sources
set(sources_SRCS 
	# HAL Core
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/dma.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/eth.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/freertos.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/gpio.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/i2c.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/main.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32f7xx_hal_msp.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32f7xx_hal_timebase_tim.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32f7xx_it.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/syscalls.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/sysmem.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/system_stm32f7xx.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/tim.c
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/usart.c

	# Drivers - STM32F7xx
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_eth.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_exti.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pcd_ex.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pcd.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart_ex.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usb.c

	# Middleware - USB CDC
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c

	# Middleware - USB Core
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c

	# Middleware - FreeRTOS
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/list.c
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/queue.c
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/timers.c
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c

	# Device - USB
	${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App/usb_device.c
	${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App/usbd_cdc_if.c
	${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App/usbd_desc.c
	${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/Target/usbd_conf.c

	# Startup
	${CMAKE_CURRENT_SOURCE_DIR}/Startup/startup_stm32f756zgtx.s

	# eMD SmartMotion Driver
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Ak0991x/Ak0991x.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948/Icm20948Augmented.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948/Icm20948AuxCompassAkm.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948/Icm20948AuxTransport.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948/Icm20948DataBaseControl.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948/Icm20948DataBaseDriver.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948/Icm20948DataConverter.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948/Icm20948Dmp3Driver.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948/Icm20948LoadFirmware.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948/Icm20948MPUFifoControl.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948/Icm20948SelfTest.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948/Icm20948Setup.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948/Icm20948Transport.c
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Sensor.c
	${INVN_EMD_SDK_PATH}/sources/Invn/DynamicProtocol/DynProtocol.c
	${INVN_EMD_SDK_PATH}/sources/Invn/DynamicProtocol/DynProtocolTransportUart.c
	${INVN_EMD_SDK_PATH}/sources/Invn/EmbUtils/DataConverter.c
	${INVN_EMD_SDK_PATH}/sources/Invn/EmbUtils/ErrorHelper.c
	${INVN_EMD_SDK_PATH}/sources/Invn/EmbUtils/InvProtocol.c
	${INVN_EMD_SDK_PATH}/sources/Invn/EmbUtils/InvQueue.c
	${INVN_EMD_SDK_PATH}/sources/Invn/EmbUtils/InvScheduler.c
	${INVN_EMD_SDK_PATH}/sources/Invn/EmbUtils/Message.c
	${INVN_EMD_SDK_PATH}/sources/Invn/EmbUtils/RingByteBuffer.c
)

# Include directories for all compilers
set(include_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
	${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F7xx/Include
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/Include
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App
	${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/Target

	# eMD SmartMotion Driver
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Ak0991x
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices/Drivers/Icm20948
	${INVN_EMD_SDK_PATH}/sources/Invn/Devices
	${INVN_EMD_SDK_PATH}/sources/Invn/DynamicProtocol
	${INVN_EMD_SDK_PATH}/sources/Invn/EmbUtils
	${INVN_EMD_SDK_PATH}/sources/Invn
)

# Include directories for each compiler
set(include_c_DIRS)
set(include_cxx_DIRS)
set(include_asm_DIRS)

# Symbols definition for all compilers
set(symbols_SYMB)

# Symbols definition for each compiler
set(symbols_c_SYMB)
set(symbols_cxx_SYMB)
set(symbols_asm_SYMB)

# Link directories and names of libraries
set(link_DIRS)
set(link_LIBS)

# Linker script
set(linker_script_SRC)

# Compiler options
set(compiler_OPTS)

# Linker options
set(linker_OPTS)

# Now call generated cmake
# This will add script generated
# information to the project
include("cmake/vscode_generated.cmake")

# Link directories setup
# Must be before executable is added
link_directories(${CMAKE_PROJECT_NAME} ${link_DIRS})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${sources_SRCS})

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${include_DIRS}
    $<$<COMPILE_LANGUAGE:C>: ${include_c_DIRS}>
    $<$<COMPILE_LANGUAGE:CXX>: ${include_cxx_DIRS}>
    $<$<COMPILE_LANGUAGE:ASM>: ${include_asm_DIRS}>
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    ${symbols_SYMB}
    $<$<COMPILE_LANGUAGE:C>: ${symbols_c_SYMB}>
    $<$<COMPILE_LANGUAGE:CXX>: ${symbols_cxx_SYMB}>
    $<$<COMPILE_LANGUAGE:ASM>: ${symbols_asm_SYMB}>

    # Configuration specific
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>: >
)

# Add linked libraries
#target_link_libraries(${CMAKE_PROJECT_NAME} ${link_LIBS})
find_package(scpi CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE manisonik::scpi)

# Compiler options
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    # ${cpu_PARAMS}
    #${compiler_OPTS}
    #-Wall
    #-Wextra
    #-Wpedantic
    #-Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:C>: >
    $<$<COMPILE_LANGUAGE:CXX>:

    # -Wno-volatile
    # -Wold-style-cast
    # -Wuseless-cast
    # -Wsuggest-override
    >
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
)

# Linker options
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -T${linker_script_SRC}
    #${cpu_PARAMS}
    ${linker_OPTS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -u _printf_float # STDIO float formatting support (remove if not used)
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,-z,max-page-size=8 # Allow good software remapping across address space (with proper GCC section making)
    -Wl,--print-memory-usage
)

# Execute post-build to print size, generate hex and bin
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
)
