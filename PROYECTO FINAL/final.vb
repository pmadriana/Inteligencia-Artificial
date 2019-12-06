Imports System
Imports System.Collections.Generic
Imports System.Collections ' for BitArray

Namespace ArtificialImmuneSystem
    Friend Class ArtificialImmuneSystemProgram
        Private Shared random As Random

        Shared Sub Main(ByVal args() As String)
            Console.WriteLine(vbcrlf & "Begin Artificial Immune System for Intrusion Detection demo" & vbcrlf)

            random = New Random(1)

            Dim numPatternBits As Integer = 12
            Dim numAntibodyBits As Integer = 4
            Dim numLymphocytes As Integer = 3
            Dim stimulationThreshold As Integer = 3

            Console.WriteLine("Loading self-antigen set ('normal' historical patterns)")
            Dim selfSet As List(Of BitArray) = LoadSelfSet(Nothing)
            ShowSelfSet(selfSet)

            Console.WriteLine(vbCrLf & "Creating lymphocyte set using multiple negative selection" & " and r-chunks detection")
            Dim lymphocyteSet As List(Of Lymphocyte) = CreateLymphocyteSet(selfSet, numAntibodyBits, numLymphocytes)
            ShowLymphocyteSet(lymphocyteSet)

            Console.WriteLine(vbcrlf & "Begin AIS intrusion detection simulation" & vbcrlf)

            Dim time As Integer = 0
            Dim maxTime As Integer = 6

            Do While time < maxTime
                Console.WriteLine("===============================================")
                Dim incoming As BitArray = RandomBitArray(numPatternBits)
                Console.WriteLine("Incoming pattern = " & BitArrayAsString(incoming) & vbcrlf)

                For i = 0 To lymphocyteSet.Count - 1
                    If lymphocyteSet(i).Detects(incoming) = True Then
                        Console.WriteLine("Incoming pattern detected by lymphocyte " & i)
                        lymphocyteSet(i).stimulation = lymphocyteSet(i).stimulation + 1
                        If lymphocyteSet(i).stimulation >= stimulationThreshold Then
                            Console.WriteLine("Lymphocyte " & i & " stimulated!" & " Check incoming as possible intrusion!")
                        Else
                            Console.WriteLine("Lymphocyte " & i & " not over stimulation threshold")
                        End If
                    Else
                        Console.WriteLine("Incoming pattern not detected by lymphocyte " & i)
                    End If
                Next i
                time = time + 1
                Console.WriteLine("===============================================")
            Loop ' while

            Console.WriteLine(vbCrLf & "End demo" & vbCrLf)
            Console.ReadLine()
        End Sub ' Main

        Public Shared Function LoadSelfSet(ByVal dataSource As String) As List(Of BitArray)
            Dim result As New List(Of BitArray)
            Dim self0() As Boolean = {True, False, False, True, False, True, True, False, True, False, False, True}
            Dim self1() As Boolean = {True, True, False, False, True, False, True, False, True, True, False, False}
            Dim self2() As Boolean = {True, False, True, True, False, False, True, True, False, True, False, True}
            Dim self3() As Boolean = {False, False, True, True, False, True, False, True, True, False, True, True}
            Dim self4() As Boolean = {False, True, False, True, False, True, False, False, True, True, False, True}
            Dim self5() As Boolean = {False, False, True, False, True, False, True, False, False, True, False, False}

            result.Add(New BitArray(self0))
            result.Add(New BitArray(self1))
            result.Add(New BitArray(self2))
            result.Add(New BitArray(self3))
            result.Add(New BitArray(self4))
            result.Add(New BitArray(self5))

            Return result
        End Function

        Public Shared Sub ShowSelfSet(ByVal selfSet As List(Of BitArray))
            For i = 0 To selfSet.Count - 1
                Console.WriteLine(i & ": " & BitArrayAsString(selfSet(i)))
            Next i
        End Sub

        Public Shared Function BitArrayAsString(ByVal ba As BitArray) As String
            Dim s As String = ""
            For i = 0 To ba.Length - 1
                If ba(i) = True Then
                    s &= 1
                Else
                    s &= 0
                End If
            Next i
            Return s
        End Function

        Public Shared Function CreateLymphocyteSet(ByVal selfSet As List(Of BitArray), ByVal numAntibodyBits As Integer, ByVal numLymphocytes As Integer) As List(Of Lymphocyte)
            ' create a List of Lymphocytes that do not detect any patterns in selfSet
            Dim result As New List(Of Lymphocyte)
            Dim contents As New Dictionary(Of Integer, Boolean) ' HashSet available in 4.5

            Do While result.Count < numLymphocytes
                Dim antibody As BitArray = RandomBitArray(numAntibodyBits) ' random antibody
                Dim lymphocyte As New Lymphocyte(antibody) ' random lymphocte
                Dim hash As Integer = lymphocyte.GetHashCode ' assumes antibody length <= 32 bits

                If DetectsAny(selfSet, lymphocyte) = False AndAlso contents.ContainsKey(hash) = False Then
                    result.Add(lymphocyte)
                    contents.Add(hash, True)
                End If
            Loop
            Return result
        End Function

        Private Shared Function DetectsAny(ByVal selfSet As List(Of BitArray), ByVal lymphocyte As Lymphocyte) As Boolean ' helper
            ' does lymphocyte detect any pattern in selfSet?
            For i = 0 To selfSet.Count - 1
                If lymphocyte.Detects(selfSet(i)) = True Then
                    Return True
                End If
            Next i
            Return False
        End Function

        Public Shared Sub ShowLymphocyteSet(ByVal lymphocyteySet As List(Of Lymphocyte))
            For i = 0 To lymphocyteySet.Count - 1
                Console.WriteLine(i & ":  " & lymphocyteySet(i).ToString)
            Next i
        End Sub

        Public Shared Function RandomBitArray(ByVal numBits As Integer) As BitArray
            Dim bools(numBits - 1) As Boolean
            For i = 0 To numBits - 1
                Dim b As Integer = random.Next(0, 2)
                bools(i) = If(b = 0, False, True)
            Next i
            Return New BitArray(bools)
        End Function

    End Class ' class ArtificialImmuneSystemProgram

    Public Class Lymphocyte
        Public antibody As BitArray ' detector
        Public searchTable() As Integer ' for fast detection
        Public age As Integer ' not used; could determine death
        Public stimulation As Integer ' controls triggering

        Public Sub New(ByVal antibody As BitArray)
            Me.antibody = New BitArray(antibody) ' assumes len >= 2
            Me.searchTable = BuildTable() ' call helper
            Me.age = 0
            Me.stimulation = 0
        End Sub

        Private Function BuildTable() As Integer()
            Dim result(antibody.Length - 1) As Integer
            Dim pos As Integer = 2
            Dim cnd As Integer = 0
            result(0) = -1
            result(1) = 0
            Do While pos < antibody.Length
                If antibody(pos - 1) = antibody(cnd) Then
                    cnd = cnd + 1
                    result(pos) = cnd
                    pos = pos + 1
                ElseIf cnd > 0 Then
                    cnd = result(cnd)
                Else
                    result(pos) = 0
                    pos = pos + 1
                End If
            Loop
            Return result
        End Function

        Public Function Detects(ByVal pattern As BitArray) As Boolean



            Dim m As Integer = 0
            Dim i As Integer = 0
            Do While m + i < pattern.Length
                If Me.antibody(i) = pattern(m + i) Then
                    If i = antibody.Length - 1 Then
                        Return True
                    End If
                    i = i + 1
                Else
                    m = m + i - Me.searchTable(i)
                    If searchTable(i) > -1 Then
                        i = searchTable(i)
                    Else
                        i = 0
                    End If
                End If
            Loop
            Return False ' not found
        End Function

        Public Overrides Function GetHashCode() As Integer
            Dim singleInt(0) As Integer
            antibody.CopyTo(singleInt, 0)
            Return singleInt(0)
        End Function

        Public Overrides Function ToString() As String
            Dim s As String = "antibody = "
            For i = 0 To antibody.Length - 1
                If antibody(i) = True Then
                    s &= 1
                Else
                    s &= 0
                End If
            Next i
            s &= " age = " & age
            s &= "  stimulation = " & stimulation
            Return s
        End Function

    End Class ' class Lymphocyte

End Namespace ' ns
