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
            Ticket ticket = new Ticket(@"C:\N\Projects\DotNet\FontTable\meta\ARDFyt0000.txt.20210928 094821");
            foreach(var name in ticket.TicketRegions.Values)
            {
                Console.WriteLine(name.RegionName);
                FontTable ft = name.FontTable;
                ft.printTable();
            }
            ticket.Parse();
        }
    }
}
