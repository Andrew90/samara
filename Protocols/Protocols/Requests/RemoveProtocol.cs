using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.SqlClient;
using System.Data;

namespace Protocols.Requests
{
    public class RemoveProtocol
    {
        protected static void RemoveProtocolsTable(int id)
        {
            string queryString =
               " DELETE t"
               + " FROM ProtocolsTable as t"
               + " WHERE t.ID =  @IDProtocols"
               ;
            using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
            {
                SqlCommand command = new SqlCommand(queryString, connection);

                command.Parameters.Add("@IDProtocols", SqlDbType.Int);
                command.Parameters["@IDProtocols"].Value = id;

                command.Connection.Open();
                command.ExecuteNonQuery();
            }
        }

        //protected static void RemoveNULLTable(string queryString)
        //{
        //    using (SqlConnection connection = new SqlConnection(Properties.Settings.Default.StoredBaseConnectionString))
        //    {
        //        SqlCommand command = new SqlCommand(queryString, connection);
        //        command.Connection.Open();
        //        command.ExecuteNonQuery();
        //    }
        //}

        public static void Do(int id)
        {
            RemoveProtocolsTable(id);
            //RemoveNULLTable(
            //            "DELETE t"
            //            + " FROM  TubesTable AS t"
            //            + " LEFT JOIN ProtocolsTable AS p"
            //            + " ON t.IDProtocolsTable = p.ID"
            //            + " WHERE p.ID IS NULL"
            //            );
            //RemoveNULLTable(
            //            " DELETE s"
            //            + " FROM StoredThicknessTable AS s"
            //            + " LEFT JOIN TubesTable AS t"
            //            + " ON t.IDProtocolsTable = s.TubesTableID"
            //            + " WHERE t.ID IS NULL"
            //            );
            //RemoveNULLTable(
            //            "DELETE s"
            //            + " FROM StoredMeshureTable AS s"
            //            + " LEFT JOIN TubesTable AS t"
            //            + " ON t.IDStoredMeshureTable = s.ID"
            //            + " WHERE t.ID IS NULL"
            //            );
            //RemoveNULLTable(
            //           "DELETE t"
            //           + " FROM ThicknessTable AS t"
            //           + " LEFT JOIN StoredThicknessTable AS s"
            //           + " ON t.ID = s.MaxThicknessID OR t.ID = s.MinThicknessID"
            //           + " WHERE s.ID IS NULL"
            //           );
            //RemoveNULLTable(
            //            "DELETE st"
            //            + " FROM StoredThresholdsTable AS st"
            //            + " LEFT JOIN StoredMeshureTable AS sm    "
            //            + " ON st.ID = sm.IDBorderAboveThickness  "
            //            + " OR st.ID = sm.IDBorderDefectCross     "
            //            + " OR st.ID = sm.IDBorderDefectLong      "
            //            + " OR st.ID = sm.IDBorderKlass2Cross     "
            //            + " OR st.ID = sm.IDBorderKlass2Long      "
            //            + " OR st.ID = sm.IDBorderLowerThickness  "
            //            + " OR st.ID = sm.IDBorderNominalThickness"
            //            + " WHERE sm.ID IS NULL                   "
            //            );
            //RemoveNULLTable(
            //           "DELETE o"
            //           + " FROM OperatorsTable AS o"
            //           + " LEFT JOIN TubesTable AS t"
            //           + " ON o.ID = t.IDOperator"
            //           + " WHERE t.ID IS NULL"
            //           );
            //
        }
    }
}
