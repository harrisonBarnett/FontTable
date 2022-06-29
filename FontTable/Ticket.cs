using FontTable.Data;
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
            string filePath = @"C:\N\Projects\DotNet\FontTable\meta\ARDFyt0000.txt.20210928 094821";
            string bitMode = "";
            bool isMetric = false;
            try
            {
                // Parse metric/imperial mode
                string firstLine = File.ReadLines(filePath).ElementAt(0);
                if (firstLine[1].ToString() == "n")
                {
                    isMetric = true;
                } else
                {
                    isMetric = false;
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

            // TEST TEST TEST
            TestDbData testData = new TestDbData();
            TicketRegions.Add(
                testData.NOMCMP,
                new TicketRegion(
                        testData.NOMCMP,
                        testData.NOMFUE,
                        testData.TMOFUE,
                        testData.NEGFUE == "N" ? false : true,
                        new int[4]
                        {
                            testData.TOPCNT,
                            testData.LFTCNT,
                            testData.ALTCNT,
                            testData.ANCCNT
                        },
                        bitMode,
                        isMetric
                    )
                ) ;
        }
    }

}
