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
            TicketRegion test = new TicketRegion(
                    "test",
                    "Arial",
                    24,
                    false,
                    new int[] { 3675, 2400, 1170, 6225 },
                    "A"
                );
            foreach(var value in test.RegionArea)
            {
                Console.WriteLine($"{value.Key}: {value.Value}");
            }
            Console.ReadLine();
        }
    }
}
