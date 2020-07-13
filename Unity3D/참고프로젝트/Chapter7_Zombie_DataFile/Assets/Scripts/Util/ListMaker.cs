using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ListMaker
{
    public static List<int> MakeListint(string input)
    {
        List<int> ret = new List<int>();
        string[] data = input.Split(';');

        for(int i = 0; i < data.Length; i++)
        {
            ret.Add(System.Convert.ToInt32(float.Parse(data[i])));
        }

        return ret;
    }
}
