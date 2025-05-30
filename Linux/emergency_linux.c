#include <linux/module.h>
#include <linux/keyboard.h>
#include <linux/input.h>
#include <linux/reboot.h>

static int ctrl_pressed = 0;
static int shift_pressed = 0;

// Ctrl + Shift + Pause/Break
static int my_keyboard_notifier(struct notifier_block *nb, unsigned long action, void *data)
{
    struct keyboard_notifier_param *param = data;

    if (action == KBD_KEYCODE)
    {
        int down = param->down;
        unsigned int value = param->value;

        // Update Ctrl state
        if (value == KEY_LEFTCTRL || value == KEY_RIGHTCTRL)
            ctrl_pressed = down;

        // Update Shift state
        if (value == KEY_LEFTSHIFT || value == KEY_RIGHTSHIFT)
            shift_pressed = down;

        // Check for Backspace press with Ctrl+Shift
        if (down && value == KEY_PAUSE && ctrl_pressed && shift_pressed)
        {
            printk(KERN_INFO "emergency_shutdown: Emergency detected! Rebooting system...\n");
            kernel_power_off();
        }
    }
    return NOTIFY_OK;
}

static struct notifier_block my_nb = {
    .notifier_call = my_keyboard_notifier
};

static int __init my_init(void)
{
    register_keyboard_notifier(&my_nb);
    printk(KERN_INFO "emergency_shutdown: Keyboard notifier registered\n");
    return 0;
}

static void __exit my_exit(void)
{
    unregister_keyboard_notifier(&my_nb);
    printk(KERN_INFO "emergency_shutdown: Keyboard notifier unregistered\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("nobody");
MODULE_DESCRIPTION("Emergency Shutdown");
