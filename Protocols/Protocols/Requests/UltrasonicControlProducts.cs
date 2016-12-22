using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data;
using System.Data.SqlClient;
using Protocols.Models;

namespace Protocols.Requests
{
    class UltrasonicControlProducts
    {
        public static IEnumerable<IDTimeCount> IDProtocolsTable(DateTime from, DateTime to)
        {
            IList<IDTimeCount> idProtocols = new List<IDTimeCount>();

            string queryString =
           "SELECT "
           + "[IDProtocolsTable]"
      //    + " , ROW_NUMBER()OVER(PARTITION BY YEAR([Date_Time])ORDER BY [Date_Time])AS X"
      + " , X "
           + ", [Date_Time], Count, op.[Operator]"
            
            +", pr.[Alloy            ]"
            +", pr.[DeliveryStatus   ]"
            +", pr.[NormativeDocument]"
            +", pr.[Gang             ]"
            +", pr.[ProductCodeNumber]"
            +", pr.[NumberPacket     ]"
            +", pr.[Standart         ]"
          
               + " FROM("
               + " SELECT [IDProtocolsTable], [Date_Time],[IDOperator]"
                 + ", COUNT(*)OVER(PARTITION BY [IDProtocolsTable]) AS Count"
                 + ", ROW_NUMBER()OVER(PARTITION BY [IDProtocolsTable] ORDER BY [Date_Time])AS N"
                 + " , ROW_NUMBER()OVER(PARTITION BY YEAR([Date_Time])ORDER BY [Date_Time])AS X"
               + " FROM [StoredBase].[dbo].[TubesTable]"
         //       + " WHERE [Date_Time] >= @_from_ AND [Date_Time] <= @_to_"
           + ")AS tmp, [StoredBase].[dbo].[OperatorsTable] AS op, [StoredBase].[dbo].[ProtocolsTable] pr"
           + " WHERE N = 1 AND tmp.[IDOperator] = op.ID AND tmp.[IDProtocolsTable] = pr.ID"
           + " AND [Date_Time] >= @_from_ AND [Date_Time] <= @_to_"
           + " ORDER BY [Date_Time], pr.[NumberPacket] DESC"
           ;
            using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
            {
                SqlCommand command = new SqlCommand(queryString, connection);

                command.Parameters.Add("@_from_", SqlDbType.DateTime);
                command.Parameters["@_from_"].Value = from;

                command.Parameters.Add("@_to_", SqlDbType.DateTime);
                command.Parameters["@_to_"].Value = to;

                command.Connection.Open();

                SqlDataReader reader = command.ExecuteReader();
                try
                {
                    while (reader.Read())
                    {
                        IDTimeCount t = new IDTimeCount();
                        t.ID = (int)reader[0];
                        t.NumberProtocol = (long)reader[1];
                        t.TteTme = (DateTime)reader[2];
                        t.Count = (int)reader[3];
                        t.Operator = (string)reader[4];

                        t.Alloy             = (string)reader[5];
                        t.DeliveryStatus    = (string)reader[6];
                        t.NormativeDocument = (string)reader[7];
                        t.Gang              = (string)reader[8];
                        t.ProductCodeNumber = (string)reader[9];
                        t.NumberPacket      = (string)reader[10];
                        t.Standart          = (string)reader[11];

                        idProtocols.Add(t);
                    }
                }
                finally
                {
                    reader.Close();
                }
            }
            return idProtocols;
        }

        public static IEnumerable<UltrasonicControlProductModels> HeaderProtocol(int id, long numberProtocol, DateTime dteTme, int count, string Operator)
        {
            List<UltrasonicControlProductModels> res = new List<UltrasonicControlProductModels>();

            string queryString = "SELECT [Alloy],[DeliveryStatus],[NormativeDocument],[Gang]"
              + ",[ProductCodeNumber],[NumberPacket],[Standart]"
              + " FROM [StoredBase].[dbo].[ProtocolsTable]"
              + "WHERE [ID]= @id"
            ;

            using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
            {
                SqlCommand command = new SqlCommand(queryString, connection);

                command.Parameters.Add("@id", SqlDbType.Int);
                command.Parameters["@id"].Value = id;

                command.Connection.Open();
                SqlDataReader reader = command.ExecuteReader();
                try
                {
                    if (reader.Read())
                    {
                        UltrasonicControlProductModels t = new UltrasonicControlProductModels();

                        t.Alloy = (string)reader[0];
                        t.DeliveryStatus = (string)reader[1];
                        t.NormativeDocument = (string)reader[2];
                        t.Gang = (string)reader[3];
                        t.ProductCodeNumber = (string)reader[4];
                        t.NumberPacket = (string)reader[5];
                        t.Standart = (string)reader[6];

                        t.Count = count;
                        t.TteTme = dteTme;
                        t.Operator = Operator;

                        t.NumberProtocol = numberProtocol.ToString();

                        res.Add(t);
                    }
                }
                finally
                {
                    reader.Close();
                }
            }
            return res;
        }
        private static double FindMin(int n0, int n1, int n2, int n3, double[] arr, int len)
        {
            double t = arr[n0];
            for (int i = 1 + n0; i < n1; ++i)
            {
                if (t > arr[i]) t = arr[i];
            }
            for (int i = n3; i < n3; ++i)
            {
                if (t > arr[i]) t = arr[i];
            }
            return t;
        }
        private static double FindMax(int n0, int n1, int n2, int n3, double[] arr, int len)
        {
            double t = arr[n0];
            for (int i = 1 + n0; i < n1; ++i)
            {
                if (t < arr[i]) t = arr[i];
            }
            for (int i = n3; i < n3; ++i)
            {
                if (t < arr[i]) t = arr[i];
            }
            return t;
        }
        public static IList<TubesPacketResult> BodyProtocol(int id)
        {
            IList<TubesPacketResult> t = new List<TubesPacketResult>();

            string queryString =
                // "SELECT t.[NumberTube],s.[LengthTube],s.[Status]"
                //+ " FROM [StoredBase].[dbo].[StoredMeshureTable] AS s,"
                //+ " [StoredBase].[dbo].[TubesTable] as t"
                //+ " WHERE  t.[IDProtocolsTable] = @IDProtocols AND t.[IDStoredMeshureTable] = s.[ID]"
                //+ " ORDER BY s.[ID] ASC"
                  "SELECT t.NumberTube, s.LengthTube, s.Status"
                + ", p.N0, p.N1, p.N1, p.N3"
                + ", (SELECT TOP 1 MinMaxThickness FROM ThicknessTable WHERE ID = st.MinThicknessID)as mn"
                + ", (SELECT TOP 1 MinMaxThickness FROM ThicknessTable WHERE ID = st.MaxThicknessID)as mx"
                + ", FROM ((TubesTable as t INNER JOIN StoredMeshureTable as s ON t.IDStoredMeshureTable = s.ID)"
                + ", INNER JOIN StoredThicknessTable as st ON st.TubesTableID = t.ID), ProtectiveThickeningTable as p"
                + ", WHERE t.IDProtocolsTable = 28 AND p.ID = st.ProtectiveThickeningTableID"
                + " ORDER BY s.ID ASC"
                ;

            using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
            {
                SqlCommand command = new SqlCommand(queryString, connection);

                command.Parameters.Add("@IDProtocols", SqlDbType.Int);
                command.Parameters["@IDProtocols"].Value = id;

                command.Connection.Open();
                SqlDataReader reader = command.ExecuteReader(CommandBehavior.SequentialAccess);
                int count;
                byte[] buffer = new byte[300];
                double[] minBuf = new double[300];
                double[] maxBuf = new double[300];
                byte[] tmpBuf = new byte[300 * sizeof(double)];
//*0*/		Clr<Undefined   >
//*1*/		, Clr<DeathZone>
//*2*/		, Clr<Nominal	>
//*3*/	    , Clr<BorderAbove<Thickness> >
//*4*/	    , Clr<BorderLower<Thickness> >
//*5*/	    , Clr<BorderKlass2<Long> > 
//*6*/	    , Clr<BorderDefect<Long> >
//*7*/	    , Clr<BorderKlass2<Cross>>
//*8*/	    , Clr<BorderDefect<Cross>>
//*9*/       , Clr<BrakStrobe2<Thickness>>
//*10*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>>
//*11*/	, Clr<BorderDefect<Cross>, BorderAbove<Thickness>>
//*12*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>>
//*13*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>
//*14*/	, Clr<BorderDefect<Long>, BorderAbove<Thickness>>
//*15*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>>
//*16*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
//*17*/	, Clr<BorderDefect<Cross>, BorderDefect<Long> >
//*18*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>>   
//*19*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>> 
//*20*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
//*21*/  , Clr<BorderKlass2<Cross>, BorderAbove<Thickness>>
//*22*/	, Clr<BorderLower<Thickness>, BorderKlass2<Cross>>
//*23*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>>
//*24*/	, Clr<BorderKlass2<Long>, BorderAbove<Thickness>>
//*25*/	, Clr<BorderLower<Thickness>, BorderKlass2<Long>>
//*26*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>>
//*27*/	, Clr<BorderKlass2<Cross>, BorderKlass2<Long> >
//*28*/  , Clr<BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>   
//*29*/  , Clr<BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>> 
//*30*/  , Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>>
//*31*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long> >
//*32*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>
//*33*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>>    
//*34*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
//*35*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross> >
//*36*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>>   
//*37*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>>      
//*38*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>> 
//*39*/	, Clr<BrakStrobe2<Thickness>, BorderAbove<Thickness> >
//*40*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness> >
//*41*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>>
//*42*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Long> > 
//*43*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long> >
//*44*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>>
//*45*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>>
//*46*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderAbove<Thickness>>
//*47*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderLower<Thickness>>
//*48*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>
//*49*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderAbove<Thickness>>
//*50*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderLower<Thickness>>
//*51*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
//*52*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long> >
//*53*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>>   
//*54*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>> 
//*55*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
//*56*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderAbove<Thickness>>
//*57*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Cross>>
//*58*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>>
//*59*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Long>, BorderAbove<Thickness>>
//*60*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Long>>
//*61*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>>
//*62*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long> >
//*63*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>   
//*64*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>> 
//*65*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>>
//*66*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long> >
//*67*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>
//*68*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>>    
//*69*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
//*70*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross> >
//*71*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>>   
//*72*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>>      
//*73*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>> 

                var cr = new []{
                    8
                    , 11
                    , 12
                    , 13
                    , 17
                    , 18
                    , 19
                    , 20
                    , 31
                    , 32
                    , 33
                    , 34
                    , 45
                    , 46
                    , 47
                    , 48
                    , 52
                    , 53
                    , 54
                    , 55
                    , 66
                    , 67
                    , 68
                    , 69
                };

                var lo = new[] { 
                      6
                    , 14
                    , 15
                    , 16
                    , 17
                    , 18
                    , 19
                    , 20
                    , 35
                    , 36
                    , 37
                    , 38
                    , 43
                    , 49
                    , 50
                    , 51
                    , 52
                    , 53
                    , 54
                    , 55
                    , 70
                    , 71
                    , 72
                    , 73
                };

                var th = new[] {
                    4
                    , 10
                    , 12
                    , 13
                    , 15
                    , 16
                    , 19
                    , 20
                    , 22
                    , 23
                    , 25
                    , 26
                    , 29
                    , 30
                    , 33
                    , 34
                    , 37
                    , 38
                    , 40
                    , 41
                    , 47
                    , 48
                    , 50
                    , 51
                    , 54
                    , 55
                    , 57
                    , 58
                    , 60
                    , 61
                    , 64
                    , 65
                    , 68
                    , 69
                    , 72
                    , 73
                };

                var b2 = new[]{
                    9
                    , 39
                    , 40
                    , 41
                    , 42
                    , 43
                    , 44
                    , 45
                    , 46
                    , 47
                    , 48
                    , 49
                    , 50
                    , 51
                    , 52
                    , 53
                    , 54
                    , 55
                    , 56
                    , 57
                    , 58
                    , 59
                    , 60
                    , 61
                    , 62
                    , 63
                    , 64
                    , 65
                    , 66
                    , 67
                    , 68
                    , 69
                    , 70
                    , 71
                    , 72
                    , 73
                };

                while (reader.Read())
                {
                    TubesPacketResult packet = new TubesPacketResult();

                    packet.NumTube = (string)reader[0];
                    packet.Length = (int)reader.GetInt32(1);
                    reader.GetBytes(2, 0, buffer, 0, 300);
                    count = packet.Length / 50;
                    string crossDef = "";
                    string longDef = "";
                    string thickDef = "";
                    string strobeDef = "";

                    byte b;

                    byte[] thick = new byte[300];
                    byte[] cross = new byte[300];
                    byte[] longA = new byte[300];
                    byte[] b2A   = new byte[300];

                    int thickCount = 0;
                    int crossCount = 0;
                    int longACount = 0;
                    int b2Count = 0;

                    for (int i = 1; i <= count; ++i)
                    {
                        b = buffer[i - 1];
                        if (cr.Contains(b))
                        {
                            cross[i - 1] = 1;
                            crossCount = i;
                        }
                        if (lo.Contains(b))
                        {
                            longA[i - 1] = 1;
                            longACount = i;
                        }
                        if (th.Contains(b))
                        {
                            thick[i - 1] = 1;
                            thickCount = i;
                        }
                        if (b2.Contains(b))
                        {
                            b2A[i - 1] = 1;
                            b2Count = i;
                        }
                    }
                    if (crossCount > 0)
                    {
                        crossDef = "Брак зоны: " + CompressString(cross, crossCount);
                    }
                    else
                    {
                        crossDef = "Годен";
                    }

                    if (longACount > 0)
                    {
                        longDef = "Брак зоны: " + CompressString(longA, longACount); //longDef;
                    } 
                    else
                    {
                        longDef = "Годен";
                    }

                    if (thickCount > 0)
                    {
                        thickDef = "Брак зоны: " + CompressString(thick, thickCount); //thickDef;
                    }
                    else
                    {
                        thickDef = "Годен";
                    }

                    if (b2Count > 0)
                    {
                        strobeDef = "Брак зоны: " + CompressString(b2A, b2Count); //strobe bit 2;
                    }
                    else
                    {
                        strobeDef = "Годен";
                    }

                    packet.Cross = crossDef;
                    packet.Long = longDef;
                    packet.Thickness = thickDef;
                    packet.StrobeBit2 = strobeDef;

                    //-минимум максимум в зоне
                    int n0 = (int)reader[3];
                    int n1 = (int)reader[4];
                    int n2 = (int)reader[5];
                    int n3 = (int)reader[6];
                    int bytesCount = count * sizeof(double);
                    reader.GetBytes(7, 0, tmpBuf, 0, bytesCount);
                    for (int i = 0; i < count; ++i )
                    {
                        minBuf[i] = BitConverter.ToDouble(tmpBuf, i * sizeof(double));
                    }
                    double findT = FindMin(n0, n1, n2, n3, minBuf, count);
                    packet.MinThickness = findT.ToString();
                    reader.GetBytes(8, 0, tmpBuf, 0, bytesCount);
                    for (int i = 0; i < count; ++i)
                    {
                        maxBuf[i] = BitConverter.ToDouble(tmpBuf, i * sizeof(double));
                    }
                    findT = FindMax(n0, n1, n2, n3, maxBuf, count);
                    packet.MaxThickness = findT.ToString();
                    //-минимум максимум в зоне конец
                    t.Add(packet);
                }
            }
            return t;
        }
        private static string CompressString(byte[] d, int length)
        {
            int tail = 1;
            int counter = 0;
            string res = "";
            for (int i = 1; i <= length; ++i)
            {
                if (d[i - 1] == d[i])
                {
                    if(1 == d[i])++counter;
                }
                else
                {
                    if (1 == d[i - 1])
                    {
                        ++counter;
                        if (counter > 2)
                        {
                            res += " ";
                            res += (1 + tail).ToString();
                            res += "-";
                            res += i.ToString();
                            res += ",";                           
                        }
                        else
                        {
                            for (int j = 0; j < counter; ++j)
                            {
                                res += " ";
                                ++tail;
                                res += tail.ToString();
                                res += ",";
                            }
                        }
                    }
                    counter = 0;
                    tail = i;
                }
            }
            return res;
        }
    }
   
}
