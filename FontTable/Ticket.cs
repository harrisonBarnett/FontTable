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
            // pull printer config data
            Console.WriteLine("Reading from file...");
            string filePath = @"C:\Users\Harrison\dev\FontTable\meta\ARDFyt0000.txt.20210928 094821";
            string bitMode = "";
            try
            {
                string line = File.ReadLines(filePath).ElementAtOrDefault(2);
                bitMode = line[3].ToString();
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
             * ]
             * )*/
        }
    }

}
