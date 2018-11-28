Imports System
Imports Tinkerforge

Module ExampleBigFont
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your LCD 128x64 Bricklet

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim lcd As New BrickletLCD128x64(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Clear display
        lcd.ClearDisplay()

        ' Write "Hello World" with big 24x32 font
        lcd.DrawText(0, 0, BrickletLCD128x64.FONT_24X32, BrickletLCD128x64.COLOR_BLACK, _
                     "24x32")

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
