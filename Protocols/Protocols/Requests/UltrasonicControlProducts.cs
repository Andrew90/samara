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
        public static IList<TubesPacketResult> BodyProtocol(int id)
        {
            IList<TubesPacketResult> t = new List<TubesPacketResult>();

            string queryString = "SELECT t.[NumberTube],s.[LengthTube],s.[Status]"
               + " FROM [StoredBase].[dbo].[StoredMeshureTable] AS s,"
               + " [StoredBase].[dbo].[TubesTable] as t"
               + " WHERE  t.[IDProtocolsTable] = @IDProtocols AND t.[IDStoredMeshureTable] = s.[ID]"
               + " ORDER BY s.[ID] ASC"
            ;

            using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
            {
                SqlCommand command = new SqlCommand(queryString, connection);

                command.Parameters.Add("@IDProtocols", SqlDbType.Int);
                command.Parameters["@IDProtocols"].Value = id;

                command.Connection.Open();
                SqlDataReader reader = command.ExecuteReader(CommandBehavior.SequentialAccess);
                long count;
                byte[] buffer = new byte[240];

                ///*4*/	, Clr<BorderLower<Thickness> >
                ///*5*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>>
                ///*6*/	, Clr<BorderKlass2<Long> > 
                ///*7*/	, Clr<BorderDefect<Long> >
                ///*8*/	, Clr<BorderKlass2<Cross>>
                ///*9*/	, Clr<BorderDefect<Cross>>
                //
                ///*10*/	, Clr<BorderDefect<Cross>, BorderAbove<Thickness>>
                ///*11*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>>
                ///*12*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>
                ///*13*/	, Clr<BorderDefect<Long>, BorderAbove<Thickness>>
                ///*14*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>>
                ///*15*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
                ///*16*/	, Clr<BorderDefect<Cross>, BorderDefect<Long> >
                ///*17*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>>   
                ///*18*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>> 
                ///*19*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
                ///*20*/  , Clr<BorderKlass2<Cross>, BorderAbove<Thickness>>
                ///*21*/	, Clr<BorderKlass2<Cross>, BorderLower<Thickness>>
                ///*22*/	, Clr<BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>
                ///*23*/	, Clr<BorderKlass2<Long>, BorderAbove<Thickness>>
                ///*24*/	, Clr<BorderKlass2<Long>, BorderLower<Thickness>>
                ///*25*/	, Clr<BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
                ///*26*/	, Clr<BorderKlass2<Cross>, BorderKlass2<Long> >
                ///*27*/  , Clr<BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>   
                ///*28*/  , Clr<BorderKlass2<Cross>, BorderKlass2<Long>, BorderLower<Thickness>> 
                ///*29*/  , Clr<BorderKlass2<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
                ///*30*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long> >
                ///*31*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>
                ///*32*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>>    
                ///*33*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
                ///*34*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross> >
                ///*35*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>>   
                ///*36*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>>      
                ///*37*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>> 

                var th = new[] { 4, 5, 11, 12, 14, 15, 18, 19, 21, 22, 24, 25, 28, 29, 32, 33, 36, 37 };

                //int num = 0;

                while (reader.Read())
                {
                    TubesPacketResult packet = new TubesPacketResult();

                    packet.NumTube = (string)reader[0];
                    packet.Length = (int)reader.GetInt32(1);
                    reader.GetBytes(2, 0, buffer, 0, 240);
                    count = packet.Length / 50;
                    string crossDef = "";
                    string longDef = "";
                    string thickDef = "";

                    //if (0 == num) num = packet.NumTube;
                    //
                    //if (num > packet.NumTube) num = packet.NumTube;

                    byte b;

                    byte[] thick = new byte[300];
                    byte[] cross = new byte[300];
                    byte[] longA = new byte[300];

                    int thickCount = 0;
                    int crossCount = 0;
                    int longACount = 0;


                    for (int i = 1; i <= count; ++i)
                    {
                        b = buffer[i - 1];
                        if ((b >= 9 && b <= 12) || (b >= 16 && b <= 19) || (b >= 30 && b <= 33))
                        {
                            //crossDef += i.ToString() + ",";
                            cross[i - 1] = 1;
                            crossCount = i;
                        }
                        if ((b == 7) || (b >= 13 && b <= 19) || (b >= 34 && b <= 37))
                        {
                           // longDef += i.ToString() + ",";
                            longA[i - 1] = 1;
                            longACount = i;
                        }
                        if (th.Contains(b))
                        {
                           // thickDef += i.ToString() + ",";
                            thick[i - 1] = 1;
                            thickCount = i;
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

                    packet.Cross = crossDef;
                    packet.Long = longDef;
                    packet.Thickness = thickDef;

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
