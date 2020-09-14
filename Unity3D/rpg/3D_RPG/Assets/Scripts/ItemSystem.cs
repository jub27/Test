using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;



public class ItemSystem : MonoBehaviour
{
    public enum ItemGrade
    {
        COMMON, UNCOMMON, EPIC, LEGENDARY
    };

    public enum ItemType
    {
        CONSUMED, WEAPON, ARMOR
    }
    [System.Serializable]
    public struct ItemData
    {
        public int id;
        public string name;
        public ItemType itemType;
        public ItemGrade itemGrade;
        public ItemData(int id, string name, ItemType itemType, ItemGrade itemGrade)
        {
            this.id = id;
            this.name = name;
            this.itemType = itemType;
            this.itemGrade = itemGrade;
        }
    }

    static public ItemSystem instance = null;
    [System.Serializable]
    public struct ItemList
    {
        public ItemData[] itemData_list;
    }
    public ItemList itemList;

    public Dictionary<int, Sprite> item_Sprites;

    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
            DontDestroyOnLoad(gameObject);
            string path = Application.dataPath + "/ItemData/itemData" + ".json";
            string jsonData = File.ReadAllText(path);
            itemList = JsonUtility.FromJson<ItemList>(jsonData);
            item_Sprites = new Dictionary<int, Sprite>();
            item_Sprites.Add(1, Resources.Load("hp", typeof(Sprite)) as Sprite);
            item_Sprites.Add(2, Resources.Load("mp", typeof(Sprite)) as Sprite);
            item_Sprites.Add(1000, Resources.Load("sword", typeof(Sprite)) as Sprite);
            item_Sprites.Add(1001, Resources.Load("sword", typeof(Sprite)) as Sprite);
            item_Sprites.Add(1002, Resources.Load("sword", typeof(Sprite)) as Sprite);
            item_Sprites.Add(1003, Resources.Load("sword", typeof(Sprite)) as Sprite);
            item_Sprites.Add(2000, Resources.Load("armor", typeof(Sprite)) as Sprite);
            item_Sprites.Add(2001, Resources.Load("armor", typeof(Sprite)) as Sprite);
            item_Sprites.Add(2002, Resources.Load("armor", typeof(Sprite)) as Sprite);
            item_Sprites.Add(2003, Resources.Load("armor", typeof(Sprite)) as Sprite);
        }
        else
        {
            Destroy(gameObject);
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        /*
        string jsonData = JsonUtility.ToJson(itemList, true);
        string path = Application.dataPath + "/ItemData/itemData" + ".json";
        File.WriteAllText(path, jsonData);
        */

    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
