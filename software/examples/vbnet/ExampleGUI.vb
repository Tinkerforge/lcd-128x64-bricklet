Imports System
Imports Tinkerforge

Module ExampleGUI
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your LCD 128x64 Bricklet

    ' Callback subroutine for gui button pressed callback
    Sub GUIButtonPressedCB(ByVal sender As BrickletLCD128x64, ByVal index As Byte, _
                           ByVal pressed As Boolean)
        Console.WriteLine("Index: " + index.ToString())
        Console.WriteLine("Pressed: " + pressed.ToString())
        Console.WriteLine("")
    End Sub

    ' Callback subroutine for gui slider value callback
    Sub GUISliderValueCB(ByVal sender As BrickletLCD128x64, ByVal index As Byte, _
                         ByVal value As Byte)
        Console.WriteLine("Index: " + index.ToString())
        Console.WriteLine("Value: " + value.ToString())
        Console.WriteLine("")
    End Sub

    ' Callback subroutine for gui tab selected callback
    Sub GUITabSelectedCB(ByVal sender As BrickletLCD128x64, ByVal index As Short)
        Console.WriteLine("Index: " + index.ToString())
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim lcd As New BrickletLCD128x64(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register gui button pressed callback to subroutine GUIButtonPressedCB
        AddHandler lcd.GUIButtonPressedCallback, AddressOf GUIButtonPressedCB

        ' Register gui slider value callback to subroutine GUISliderValueCB
        AddHandler lcd.GUISliderValueCallback, AddressOf GUISliderValueCB

        ' Register gui tab selected callback to subroutine GUITabSelectedCB
        AddHandler lcd.GUITabSelectedCallback, AddressOf GUITabSelectedCB

        ' Clear display
        lcd.ClearDisplay()
        lcd.RemoveAllGUI()

        ' Add GUI elements: Button, Slider and Graph with 60 data points
        lcd.SetGUIButton(0, 0, 0, 60, 20, "button")
        lcd.SetGUISlider(0, 0, 30, 60, BrickletLCD128x64.DIRECTION_HORIZONTAL, 50)
        lcd.SetGUIGraphConfiguration(0, BrickletLCD128x64.GRAPH_TYPE_LINE, 62, 0, 60, _
                                     52, "X", "Y")

        ' Add a few data points (the remaining points will be 0)
        lcd.SetGUIGraphData(0, _
                            new Byte(){20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240})

        ' Add 5 text tabs without and configure it for click and swipe without auto-redraw
        lcd.SetGUITabConfiguration(BrickletLCD128x64.CHANGE_TAB_ON_CLICK_AND_SWIPE, _
                                   False)
        lcd.SetGUITabText(0, "Tab A")
        lcd.SetGUITabText(1, "Tab B")
        lcd.SetGUITabText(2, "Tab C")
        lcd.SetGUITabText(3, "Tab D")
        lcd.SetGUITabText(4, "Tab E")

        ' Set period for gui button pressed callback to 0.1s (100ms)
        lcd.SetGUIButtonPressedCallbackConfiguration(100, True)

        ' Set period for gui slider value callback to 0.1s (100ms)
        lcd.SetGUISliderValueCallbackConfiguration(100, True)

        ' Set period for gui tab selected callback to 0.1s (100ms)
        lcd.SetGUITabSelectedCallbackConfiguration(100, True)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
