#ifndef ADAFRUIT_RGB_LCD_H
#define ADAFRUIT_RGB_LCD_H

#include <linux/i2c.h>

enum {
        /* The I2C address for the MCP23017 GPIO expander */
        MCP23017_ADDRESS = 0x20,
        LCD_WRITE_ATTEMPTS = 1024,
        LCD_MAX_ESCAPE_PREFIXES = 4,
};

/* Device register pairs available on the MCP23017 */
typedef enum
{
        MCP23017_IODIR    = 0x00,
        MCP23017_IPOL     = 0x02,
        MCP23017_GPINTEN  = 0x04,
        MCP23017_DEFVAL   = 0x06,
        MCP23017_INTCON   = 0x08,
        MCP23017_IOCON    = 0x0A,
        MCP23017_GPPU     = 0x0C,
        MCP23017_INTF     = 0x0E,
        MCP23017_INTCAP   = 0x10,
        MCP23017_GPIO     = 0x12,
        MCP23017_OLAT     = 0x14,
} reg;

/* 3-bit color values that the backlight can be set to */
typedef enum
{
        LED_COLOR_OFF      = 0x0,
        LED_COLOR_RED      = 0x1,
        LED_COLOR_GREEN    = 0x2,
        LED_COLOR_BLUE     = 0x4,
        LED_COLOR_YELLOW   = 0x3,
        LED_COLOR_TEAL     = 0x6,
        LED_COLOR_VIOLET   = 0x5,
        LED_COLOR_ON       = 0x7,
} led_color;

/* Buttons available on the device, and their "IDs" */
typedef enum
{
        BUTTON_SELECT   = 0,
        BUTTON_RIGHT    = 1,
        BUTTON_DOWN     = 2,
        BUTTON_UP       = 3,
        BUTTON_LEFT     = 4,
} button;

/* Command masks accepted by the LCD */
typedef enum
{
        LCD_CLEARDISPLAY   = 0x01,
        LCD_RETURNHOME     = 0x02,
        LCD_ENTRYMODESET   = 0x04,
        LCD_DISPLAYCONTROL = 0x08,
        LCD_CURSORSHIFT    = 0x10,
        LCD_FUNCTIONSET    = 0x20,
        LCD_SETCGRAMADDR   = 0x40,
        LCD_SETDDRAMADDR   = 0x80,
} lcd_command;

typedef enum
{
        LCD_ENTRYRIGHT          = 0x00,
        LCD_ENTRYLEFT           = 0x02,
        LCD_ENTRYSHIFTINCREMENT = 0x01,
        LCD_ENTRYSHIFTDECREMENT = 0x00,
} lcd_entry_flags;

typedef enum
{
        LCD_DISPLAYON    = 0x04,
        LCD_DISPLAYOFF   = 0x00,
        LCD_CURSORON     = 0x02,
        LCD_CURSOROFF    = 0x00,
        LCD_BLINKON      = 0x01,
        LCD_BLINKOFF     = 0x00,
} lcd_display_flags;

typedef enum
{
        LCD_DISPLAYMOVE = 0x08,
        LCD_CURSORMOVE  = 0x00,
        LCD_MOVERIGHT   = 0x04,
        LCD_MOVELEFT    = 0x00,
} lcd_move_flags;

typedef enum
{
        LCD_8BITMODE = 0x10,
        LCD_4BITMODE = 0x00,
        LCD_2LINE    = 0x08,
        LCD_1LINE    = 0x00,
        LCD_5x10DOTS = 0x04,
        LCD_5x8DOTS  = 0x00,
} lcd_mode;

typedef union
{
        struct
        {
                bool button_select : 1;
                bool button_right : 1;
                bool button_down : 1;
                bool button_up : 1;
                bool button_left : 1;
                bool unused0 : 1;
                bool color_r : 1;
                bool color_g : 1;
                bool color_b : 1;
                bool lcd_data0 : 1;
                bool lcd_data1 : 1;
                bool lcd_data2 : 1;
                bool lcd_data3 : 1;
                bool lcd_enable : 1;
                bool lcd_rw : 1;
                bool lcd_rs : 1;
        };
        struct
        {
                unsigned buttons : 5;
                unsigned unused : 1;
                unsigned color : 3;
                unsigned lcd_data : 4;
                unsigned lcd_control : 3;
        };
        struct
        {
                uint8_t value_a;
                uint8_t value_b;
        };
        uint16_t value;
} reg_value;

typedef enum
{
        LCD_ESCAPE_NONE = 0,
        LCD_ESCAPE_SINGLE = 1,
        LCD_ESCAPE_SEQUENCE = 2,
} lcd_escape_level;

typedef struct
{
        int number;
        struct i2c_client *client;
        /* TODO: Locking during writes */

        reg_value iodir;
        reg_value gppu;
        reg_value gpio;
        reg_value olat;

        led_color backlight_color;

        /* Col is int instead of e.g. uint8_t, because of ANSI cursor
         * positioning commands that we don't want to make the cursor
         * wrap
         */
        int lcd_col;
        int lcd_saved_col;
        /* TODO: implement corresponding escape
         * seqs for saving cursor pos
         */
        bool lcd_row;
        bool lcd_saved_row;
        lcd_escape_level lcd_escape;
        uint8_t lcd_esc_pref[LCD_MAX_ESCAPE_PREFIXES];
        size_t lcd_esc_pref_pos;
} device_data;


static int mod_init (void);
static void mod_exit (void);

static int driver_probe (struct i2c_client *, const struct i2c_device_id *);
static int driver_remove (struct i2c_client *);

static int driver_detect (struct i2c_client *, struct i2c_board_info *);

static ssize_t chardev_read (struct file *, char __user *, size_t, loff_t *);
static ssize_t chardev_write (struct file *, const char __user *, size_t, loff_t *);

static int chardev_open (struct inode *, struct file *);
static int chardev_release (struct inode *, struct file *);

static int set_backlight (device_data *, led_color);

static ssize_t set_backlight_attr (struct device *, struct device_attribute *, const char *, size_t);
static ssize_t get_backlight_attr (struct device *, struct device_attribute *, char *);

static int sync_cursorpos (device_data *);

static int send_command (device_data *, uint8_t);
static int send_char (device_data *, uint8_t);

static int write_data (device_data *, bool, uint8_t);

static int wait_for_write (device_data *);

static int set_reg_pair (struct i2c_client *, reg, reg_value *, reg_value);
static int set_reg (struct i2c_client *, reg, bool, reg_value *, reg_value);
static int get_reg_pair (struct i2c_client *, reg, reg_value *);
static int get_reg (struct i2c_client *, reg, bool, reg_value *);

#endif /* H */
