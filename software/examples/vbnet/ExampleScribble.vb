Imports System
Imports System.Drawing
Imports System.Math
Imports System.Threading
Imports Tinkerforge

Module ExampleScribble
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your LCD 128x64 Bricklet
    Const WIDTH As Integer = 128
    Const HEIGHT As Integer = 64

    Sub DrawImage(ByRef lcd As BrickletLCD128x64, ByVal bitmap As Bitmap)
        Dim pixels() As Boolean = New Boolean(HEIGHT*WIDTH) {}

        For row As Integer = 0 To HEIGHT - 1
            For column As Integer = 0 To WIDTH - 1
                pixels(row*WIDTH + column) = bitmap.GetPixel(column, row).GetBrightness() > 0
            Next column
        Next row

        lcd.WritePixels(0, 0, WIDTH-1, HEIGHT-1, pixels)
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim lcd As New BrickletLCD128x64(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Clear display
        lcd.ClearDisplay()

        ' Draw rotating line
        Dim bitmap As New Bitmap(WIDTH, HEIGHT)
        Dim originX As Integer = WIDTH \ 2
        Dim originY As Integer = HEIGHT \ 2
        Dim length As Integer = HEIGHT \ 2 - 2
        Dim angle As Integer = 0

        Console.WriteLine("Press enter to exit")

        While Not Console.KeyAvailable
            Dim radians As Double = Math.PI * angle / 180.0
            Dim x As Integer = Convert.ToInt32(Math.Floor(originX + length * Math.Cos(radians)))
            Dim y As Integer = Convert.ToInt32(Math.Floor(originY + length * Math.Sin(radians)))

            Using g As Graphics = Graphics.FromImage(bitmap)
                g.FillRectangle(Brushes.Black, 0, 0, WIDTH, HEIGHT)
                g.DrawLine(Pens.White, originX, originY, x, y)
            End Using

            DrawImage(lcd, bitmap)
            Thread.Sleep(25)

            angle += 1
        End While

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
