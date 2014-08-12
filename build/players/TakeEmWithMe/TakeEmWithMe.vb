Module Module1
    Dim players As String()
    Dim numTurns As Integer

Public Sub Main(args() As String)
    'split results into the different players
    players = args(1).Split(" ")
    'how many turns have we been going?
    numTurns = players(0).Split(",").Length
    If numTurns = 48 Then 'can hear the truck
        Console.Write("P")
    ElseIf numTurns = 49 Then ' can see the truck
        Console.Write("T" & PlayerWithHighestHealth())
    ElseIf numTurns = 50 Then 'can smell the truck
        Console.Write("S0")
    Else ' hiding is best
        Console.Write("D" & PlayerWithHighestHealth())
    End If
End Sub

Private Function PlayerWithHighestHealth() As Integer
    Dim p As Integer = 1
    Dim health As Integer = 1
    For i = 1 To 3
        If players(i).Split(",")(0) > health Then 'this is the healthiest looking guy
            p = i
            health = players(i).Split(",")(0)
        End If
    Next
    Return p
End Function

End Module
