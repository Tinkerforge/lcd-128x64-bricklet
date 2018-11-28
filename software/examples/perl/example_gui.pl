#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLCD128x64;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your LCD 128x64 Bricklet

# Callback subroutine for gui button pressed callback
sub cb_gui_button_pressed
{
    my ($index, $pressed) = @_;

    print "Index: $index\n";
    print "Pressed: $pressed\n";
    print "\n";
}

# Callback subroutine for gui slider value callback
sub cb_gui_slider_value
{
    my ($index, $value) = @_;

    print "Index: $index\n";
    print "Value: $value\n";
    print "\n";
}

# Callback subroutine for gui tab selected callback
sub cb_gui_tab_selected
{
    my ($index) = @_;

    print "Index: $index\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $lcd = Tinkerforge::BrickletLCD128x64->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register gui button pressed callback to subroutine cb_gui_button_pressed
$lcd->register_callback($lcd->CALLBACK_GUI_BUTTON_PRESSED, 'cb_gui_button_pressed');

# Register gui slider value callback to subroutine cb_gui_slider_value
$lcd->register_callback($lcd->CALLBACK_GUI_SLIDER_VALUE, 'cb_gui_slider_value');

# Register gui tab selected callback to subroutine cb_gui_tab_selected
$lcd->register_callback($lcd->CALLBACK_GUI_TAB_SELECTED, 'cb_gui_tab_selected');

# Clear display
$lcd->clear_display();
$lcd->remove_all_gui();

# Add GUI elements: Button, Slider and Graph with 60 data points
$lcd->set_gui_button(0, 0, 0, 60, 20, "button");
$lcd->set_gui_slider(0, 0, 30, 60, $lcd->DIRECTION_HORIZONTAL, 50);
$lcd->set_gui_graph_configuration(0, $lcd->GRAPH_TYPE_LINE, 62, 0, 60, 52, "X", "Y");

# Add a few data points (the remaining points will be 0)
$lcd->set_gui_graph_data(0, [20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240]);

# Add 5 text tabs without and configure it for click and swipe without auto-redraw
$lcd->set_gui_tab_configuration($lcd->CHANGE_TAB_ON_CLICK_AND_SWIPE, 0);
$lcd->set_gui_tab_text(0, "Tab A");
$lcd->set_gui_tab_text(1, "Tab B");
$lcd->set_gui_tab_text(2, "Tab C");
$lcd->set_gui_tab_text(3, "Tab D");
$lcd->set_gui_tab_text(4, "Tab E");

# Set period for gui button pressed callback to 0.1s (100ms)
$lcd->set_gui_button_pressed_callback_configuration(100, 1);

# Set period for gui slider value callback to 0.1s (100ms)
$lcd->set_gui_slider_value_callback_configuration(100, 1);

# Set period for gui tab selected callback to 0.1s (100ms)
$lcd->set_gui_tab_selected_callback_configuration(100, 1);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
