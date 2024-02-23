Imports System
Imports System.Math
Imports Tinkerforge

Module ExamplePixelMatrix
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your LCD 128x64 Bricklet
    Const WIDTH As Integer = 128
    Const HEIGHT As Integer = 64

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim lcd As New BrickletLCD128x64(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Clear display
        lcd.ClearDisplay()

        ' Draw checkerboard pattern
        Dim pixels() As Boolean = New Boolean(HEIGHT*WIDTH) {}

        For row As Integer = 0 To HEIGHT - 1
            For column As Integer = 0 To WIDTH - 1
                pixels(row*WIDTH + column) = (row \ 8) Mod 2 = (column \ 8) Mod 2
            Next column
        Next row

        lcd.WritePixels(0, 0, WIDTH-1, HEIGHT-1, pixels)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
