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
        public static IList<IDTimeCount> IDProtocolsTable(DateTime from, DateTime to)
        {
            IList<IDTimeCount> idProtocols = new List<IDTimeCount>();

            string queryString = "SELECT [IDProtocolsTable], [Date_Time], Count"
               + " FROM(SELECT [IDProtocolsTable], [Date_Time]"
                 + ", COUNT(*)OVER(PARTITION BY [IDProtocolsTable]) AS Count"
                 + ", ROW_NUMBER()OVER(PARTITION BY [IDProtocolsTable] ORDER BY [Date_Time])AS N"
               + " FROM [StoredBase].[dbo].[TubesTable]"
               + " WHERE [Date_Time] > @_from_ AND [Date_Time] < @_to_"
           + ")AS tmp WHERE N = 1 ORDER BY [Date_Time] DESC"
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
                        t.TteTme = (DateTime)reader[1];
                        t.Count = (int)reader[2];
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
        /*
        public class UltrasonicControlProductModels
        {
            public int      ID                { get; set; }
            public string   Alloy             { get; set; }
            public string   DeliveryStatus    { get; set; }
            public string   NormativeDocument { get; set; }
            public string   Gang              { get; set; }
            public string   ProductCodeNumber { get; set; }
            public string   NumberPacket      { get; set; }
            public string   Standart          { get; set; }
            public int      Count             { get; set; }
            public DateTime TteTme            { get; set; }
        }
*/
        public static UltrasonicControlProductModels HeaderProtocol(int id, DateTime dteTme, int count)
        {
            UltrasonicControlProductModels t = null;

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
                        t = new UltrasonicControlProductModels();

                        t.Alloy = (string)reader[0];
                        t.DeliveryStatus = (string)reader[1];
                        t.NormativeDocument = (string)reader[2];
                        t.Gang = (string)reader[3];
                        t.ProductCodeNumber = (string)reader[4];
                        t.NumberPacket = (string)reader[5];
                        t.Standart = (string)reader[6];

                        t.Count = count;
                        t.TteTme = dteTme;
                    }
                }
                finally
                {
                    reader.Close();
                }
            }

            return t;
        }
    }   
}
