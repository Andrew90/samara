using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.SqlClient;
using Microsoft.ReportingServices.DataProcessing;
using System.Data;
using Protocols.Models;
using System.Xml.Serialization;
using System.IO;

namespace Protocols.Requests
{
    public class SaveToXML
    {
        static public void Save(int id, string tubeParty, string path)
        {
            string queryString =
                 "SELECT t.NumberTube, s.LengthTube"
               + ", (SELECT TOP 1 MinMaxThickness FROM ThicknessTable WHERE ID = st.MinThicknessID)as mn"
               + ", (SELECT TOP 1 MinMaxThickness FROM ThicknessTable WHERE ID = st.MaxThicknessID)as mx"
               + " FROM ((TubesTable as t INNER JOIN StoredMeshureTable as s ON t.IDStoredMeshureTable = s.ID)"
               + " INNER JOIN StoredThicknessTable as st ON st.TubesTableID = t.ID)"
               + " WHERE t.IDProtocolsTable = @IDProtocols"
               + " ORDER BY s.ID ASC"
               ;
            using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
            {
                SqlCommand command = new SqlCommand(queryString, connection);

                command.Parameters.Add("@IDProtocols", SqlDbType.Int);
                command.Parameters["@IDProtocols"].Value = id;

                command.Connection.Open();
                SqlDataReader reader = command.ExecuteReader(System.Data.CommandBehavior.SequentialAccess);
                byte[] tmpBuf0 = new byte[300 * sizeof(double)];
                byte[] tmpBuf1 = new byte[300 * sizeof(double)];
                PacketXML packet = new PacketXML();
                packet.TubeParty = tubeParty;
                while (reader.Read())
                {
                    Tube tube = new Tube();
                    tube.Number = (int)reader[0];
                    int length = (int)reader[1];
                    List<Zone> zones = new List<Zone>();
                    tube.Zones = zones;
                    reader.GetBytes(2, 0, tmpBuf0, 0, length * sizeof(double));
                    reader.GetBytes(3, 0, tmpBuf0, 1, length * sizeof(double));

                    for (int i = 0; i < length; ++i)
                    {
                        Zone zone = new Zone();
                        zone.MinVal = BitConverter.ToDouble(tmpBuf0, i * sizeof(double));
                        zone.MaxVal = BitConverter.ToDouble(tmpBuf1, i * sizeof(double));
                        zone.Number = i;
                        zones.Add(zone);
                    }

                    packet.Tubes.Add(tube);
                }

                XmlSerializer o = new XmlSerializer(typeof(PacketXML));
                TextWriter WriteFileStream  = new StreamWriter(path);
                o.Serialize(WriteFileStream, packet);
                WriteFileStream.Close();
            }
        }
    }
}
