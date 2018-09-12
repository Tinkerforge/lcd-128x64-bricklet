Imports System
Imports Tinkerforge

Module ExampleTouch
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your LCD 128x64 Bricklet

    ' Callback subroutine for touch_position callback
    Sub TouchPositionCB(ByVal sender As BrickletLCD128x64, ByVal pressure As Integer, _
                        ByVal x As Integer, ByVal y As Integer, ByVal age As Long)
        Console.WriteLine("Pressure: " + pressure.ToString())
        Console.WriteLine("X: " + x.ToString())
        Console.WriteLine("Y: " + y.ToString())
        Console.WriteLine("Age: " + age.ToString())
        Console.WriteLine("")
    End Sub

    ' Callback subroutine for touch_gesture callback
    Sub TouchGestureCB(ByVal sender As BrickletLCD128x64, ByVal gesture As Byte, _
                       ByVal duration As Long, ByVal xStart As Integer, _
                       ByVal xEnd As Integer, ByVal yStart As Integer, _
                       ByVal yEnd As Integer, ByVal age As Long)
        Console.WriteLine("Gesture: " + gesture.ToString())
        Console.WriteLine("Duration: " + duration.ToString())
        Console.WriteLine("X Start: " + xStart.ToString())
        Console.WriteLine("X End: " + xEnd.ToString())
        Console.WriteLine("Y Start: " + yStart.ToString())
        Console.WriteLine("Y End: " + yEnd.ToString())
        Console.WriteLine("Age: " + age.ToString())
        Console.WriteLine("")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim lcd As New BrickletLCD128x64(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register touch_position callback to subroutine TouchPositionCB
        AddHandler lcd.TouchPositionCallback, AddressOf TouchPositionCB

        ' Register touch_gesture callback to subroutine TouchGestureCB
        AddHandler lcd.TouchGestureCallback, AddressOf TouchGestureCB

        ' Configure touch position callback with a period of 100ms
        lcd.SetTouchPositionCallbackConfiguration(100, True)

        ' Configure touch gesture callback with a period of 100ms
        lcd.SetTouchGestureCallbackConfiguration(100, True)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
