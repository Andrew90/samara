using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Protocols.Models
{
    public class IDTimeCount
    {
        public int ID { get; set; }
        public long NumberProtocol { get; set; }
        public DateTime TteTme { get; set; }
        public int Count { get; set; }
        public string Operator { get; set; }

        public string Alloy              { get; set; }
        public string DeliveryStatus     { get; set; }
        public string NormativeDocument  { get; set; }
        public string Gang               { get; set; }
        public string ProductCodeNumber  { get; set; }
        public string NumberPacket       { get; set; }
        public string Standart           { get; set; }
    }

    public class UltrasonicControlProductModels
    {
        public int      ID               { get; set; }
        public string   Alloy            { get; set; }
        public string   DeliveryStatus   { get; set; }
        public string   NormativeDocument{ get; set; }
        public string   Gang             { get; set; }
        public string   ProductCodeNumber{ get; set; }
        public string   NumberPacket     { get; set; }
        public string   Standart         { get; set; }
        public int      Count            { get; set; }
        public DateTime TteTme           { get; set; }
        public string   Operator         { get; set; }
        public string NumberProtocol { get; set; }
    }

    public class TubesPacketResult
    {
        public string NumTube      { get; set; }
        public int    Length    { get; set; }
        public string Long      { get; set; }
        public string Cross     { get; set; }
        public string Thickness { get; set; }
    }
}
