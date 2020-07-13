using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
using UnityEngine;

public class ItemDataLoader
{
    private Dictionary<int, ItemData> dic = new Dictionary<int, ItemData>();
    private bool isInit = false;

    public Dictionary<int, ItemData> GetDic()
    {
        if (!isInit)
            InitDic();

        return dic;
    }

    public void InitDic()
    {
        TextAsset text = Resources.Load<TextAsset>("Data\\Item");
        string[] lines = text.text.Split('\n');

        for (int i = 0; i < lines.Length - 3; i++)
        {
            string line = lines[i + 3];
            ItemData data = ScriptableObject.CreateInstance<ItemData>();

            if (line == "")
                break;

            if (line == "\r")
                break;

            data.Index = System.Convert.ToInt32(float.Parse(line.Split(',')[0]));
            data.Name = line.Split(',')[1];
            data.ItemValue = ListMaker.MakeListint(line.Split(',')[2]);

            dic.Add(data.Index, data);
        }
    }
}
