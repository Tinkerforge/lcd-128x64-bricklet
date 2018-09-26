Imports System
Imports Tinkerforge

Module ExampleTouch
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your LCD 128x64 Bricklet

    ' Callback subroutine for touch position callback
    Sub TouchPositionCB(ByVal sender As BrickletLCD128x64, ByVal pressure As Integer, _
                        ByVal x As Integer, ByVal y As Integer, ByVal age As Long)
        Console.WriteLine("Pressure: " + pressure.ToString())
        Console.WriteLine("X: " + x.ToString())
        Console.WriteLine("Y: " + y.ToString())
        Console.WriteLine("Age: " + age.ToString())
        Console.WriteLine("")
    End Sub

    ' Callback subroutine for touch gesture callback
    Sub TouchGestureCB(ByVal sender As BrickletLCD128x64, ByVal gesture As Byte, _
                       ByVal duration As Long, ByVal pressureMax As Integer, _
                       ByVal xStart As Integer, ByVal xEnd As Integer, _
                       ByVal yStart As Integer, ByVal yEnd As Integer, _
                       ByVal age As Long)
        Console.WriteLine("Gesture: " + gesture.ToString())
        Console.WriteLine("Duration: " + duration.ToString())
        Console.WriteLine("Pressure Max: " + pressureMax.ToString())
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

        ' Register touch position callback to subroutine TouchPositionCB
        AddHandler lcd.TouchPositionCallback, AddressOf TouchPositionCB

        ' Register touch gesture callback to subroutine TouchGestureCB
        AddHandler lcd.TouchGestureCallback, AddressOf TouchGestureCB

        ' Set period for touch position callback to 0.1s (100ms)
        lcd.SetTouchPositionCallbackConfiguration(100, True)

        ' Set period for touch gesture callback to 0.1s (100ms)
        lcd.SetTouchGestureCallbackConfiguration(100, True)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
