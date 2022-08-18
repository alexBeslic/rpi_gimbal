#include "accel.h"

static int file_i2c;

/**
 * @brief Opens I2C bus and sets slave addres (SAD)
 * 
 * @return uint8_t (0 - on success; 1 - on failure)
 */
uint8_t open_i2c_bus()
{
    if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		perror("open");
		return 1;
	}

	if (ioctl(file_i2c, I2C_SLAVE, SAD) < 0)
	{
		perror("ioctl");
		return 1;
	}

    return 0;
}

/**
 * @brief Configuring accel
 * 
 * @return uint8_t (0 - on success; 1 - on failure)
 */
uint8_t accel_config()
{

    // Setting up CTRL 1

    uint8_t data[2] = {CTRL1_ADDR, CTRL1_REG};
    uint8_t length = sizeof(data);

    if (write(file_i2c, data, length) != length)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
        return 1;
	}

    // Setting up CTRL 6

    data[0] = CTRL6_ADDR;
    data[1] = CTRL6_REG;

    if (write(file_i2c, data, length) != length)
	{
		/* ERROR HANDLING: i2c transaction failed */
		perror("write");
        return 1;
	}

    return 0;
}

/**
 * @brief Reads XYZ
 * 
 * @return uint8_t (0 - on success; 1 - on failure; 2 - Data not ready)
 */
uint8_t read_accel_xyz()
{
    uint8_t data;
    uint8_t accel_data[6];
	uint8_t write_addr = STATUS_ADDR;
    uint8_t length = sizeof(write_addr);

    // Tell the sensor we want to read Status reg
    if (write(file_i2c, &write_addr, length) != length)
	{
		/* ERROR HANDLING: i2c transaction failed */
		perror("write");
        return 1;
	}
    else
    {
        // Read Status
        if (read(file_i2c, &data, length) != length)
	    {
		    //ERROR HANDLING: i2c transaction failed
		    perror("read");
            return 1;
        }
        else
        {
            // Check Data-ready status.
            if (data & 0x01)
            {
                // Tell the sensor we want to read XYZ
                write_addr = X_OUT_L_ADDR;

                if (write(file_i2c, &write_addr, length) != length)
                {
                    /* ERROR HANDLING: i2c transaction failed */
                    perror("write");
                    return 1;
                }
                else
                {
                    // Read XYZ
                    length = sizeof(accel_data);

                    if (read(file_i2c, accel_data, length) != length)
                    {
                        //ERROR HANDLING: i2c transaction failed
                        perror("read");
                        return 1;
                    }
                    else
                    {
                        // Extract Data
                        int16_t x_data = (accel_data[1] << 8) | accel_data[0];
                        int16_t y_data = (accel_data[3] << 8) | accel_data[2];
                        int16_t z_data = (accel_data[5] << 8) | accel_data[4];

                        ACCEL_XYZ.x = ((x_data / 4) * CONV_CONST);
                        ACCEL_XYZ.y = ((y_data / 4) * CONV_CONST);
                        ACCEL_XYZ.z = ((z_data / 4) * CONV_CONST);
                    }
                    
                }
            }
            else
            {
                // Data not ready
                return 2;
            }
        }
    }

    return 0;
}