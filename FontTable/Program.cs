using System;
using System.IO;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;


namespace FontTable
{
    internal class Program
    {
        static void Main(string[] args)
        {
            TicketParser parser = new TicketParser(@"C:\N\Projects\DotNet\FontTable\meta\ARDFyt0000.txt.20210928 094821");
            foreach(var name in parser._ticket.TicketRegions.Values)
            {
                Console.WriteLine(name.RegionName);
                FontTable ft = name.FontTable;
                ft.printTable();
            }
            
        }
    }
}
