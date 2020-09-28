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

    static public ItemSystem instance = null;
    [System.Serializable]
    public struct ConsumeData
    {
        public int id;
        public string name;
        public int itemType;
        public int itemGrade;
        public string itemInfo;
        public int price;
        public int point;
    }
    [System.Serializable]
    public struct WeaponData
    {
        public int id;
        public string name;
        public int itemType;
        public int itemGrade;
        public string itemInfo;
        public int price;
        public float attack;
    }
    [System.Serializable]
    public struct ArmorData
    {
        public int id;
        public string name;
        public int itemType;
        public int itemGrade;
        public string itemInfo;
        public int price;
        public float defense;
    }

    [System.Serializable]
    public struct ItemList
    {
        public ConsumeData[] consumeData_list;
        public WeaponData[] weaponData_list;
        public ArmorData[] armorData_list;

    }

    public ItemList itemList;
    public Dictionary<int, Sprite> item_Sprites;
    public Dictionary<int, ConsumeData> consume_dict;
    public Dictionary<int, WeaponData> weapon_dict;
    public Dictionary<int, ArmorData> armor_dict;
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


            consume_dict = new Dictionary<int, ConsumeData>();
            weapon_dict = new Dictionary<int, WeaponData>();
            armor_dict = new Dictionary<int, ArmorData>();
            for(int i = 0; i < itemList.consumeData_list.Length; i++)
            {
                consume_dict.Add(itemList.consumeData_list[i].id, itemList.consumeData_list[i]);
            }
            for (int i = 0; i < itemList.weaponData_list.Length; i++)
            {
                weapon_dict.Add(itemList.weaponData_list[i].id, itemList.weaponData_list[i]);
            }
            for (int i = 0; i < itemList.armorData_list.Length; i++)
            {
                armor_dict.Add(itemList.armorData_list[i].id, itemList.armorData_list[i]);
            }

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
