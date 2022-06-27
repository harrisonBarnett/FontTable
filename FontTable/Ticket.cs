using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace FontTable
{
    public class Ticket
    {
        #region Properties
        public Dictionary<string, TicketRegion> TicketRegions;
        #endregion  
        public Ticket()
        {
            LoadTicket();
        }
        private void LoadTicket()
        {
            // TODO: load ticketdata from file
            // TODO: get inch vs. metric mode from line 1 (bool isMetric)
            // TODO: get DPI information from ticketprinter (int DPI)
            // pull printer config data
            // Sample ticket data:
            // <STX>n           <-- n = inch mode, where units are measured as 1/100 inch
            // <STX>M3417       <-- M = maximum label length, e.g. 3417 units (34 inches)
            // <STX>ICAFgfx0    <-- I = init image data; C = default memory module; A = datatype (7bit ASCII); F = format designator (7-bit Datamax-O'Neil); 
            Console.WriteLine("Reading from file...");
            string filePath = @"C:\Users\Harrison\dev\FontTable\meta\ARDFyt0000.txt.20210928 094821";
            string bitMode = "";
            bool isMetric = false;
            int ticketLength;
            try
            {
                // Parse metric/imperial mode
                string firstLine = File.ReadLines(filePath).ElementAt(0);
                if (firstLine[1].ToString() == "n")
                {
                    isMetric = true;
                }
                // Parse bitmode from datatype
                string thirdLine = File.ReadLines(filePath).ElementAtOrDefault(2);
                bitMode = thirdLine[3].ToString();
            }
            catch (Exception e)
            {
                Console.WriteLine("The process failed: {0}", e.ToString());
            }
            // TODO: load ticketdata fields from database
            // for each row, newup a TicketRegion
            /* new TicketRegion(
             * ticketdata.NOMCMP,
             * ticketdata.NOMFUE,
             * ticketdata.TMOFUE,
             * ticketdata.NEGFUE,
             * [
                 * ticketdata.TOPCNT,
                 * ticketdata.LFTCNT,
                 * ticketdata.ANCCNT,
                 * ticketdata.ALTCNT,
             * ],
             * bitmode,
             * isMetric,
             * DPI
             * )*/
        }
    }

}
