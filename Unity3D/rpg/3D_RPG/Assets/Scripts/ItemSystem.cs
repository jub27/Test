using System.Collections;
using System.Collections.Generic;
using UnityEngine;

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
    public struct ItemData
    {
        public int num;
        public string name;
        public ItemType itemType;
        public ItemGrade itemGrade;
        public ItemData(int num, string name, ItemType itemType, ItemGrade itemGrade)
        {
            this.num = num;
            this.name = name;
            this.itemType = itemType;
            this.itemGrade = itemGrade;
        }
    }

    static public ItemSystem instance = null;
    public ArrayList itemList = new ArrayList();
    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
            DontDestroyOnLoad(gameObject);
            for(int i = 0; i < 100; i++)
            {
                
                itemList.Add(new ItemData(i, "item" + i, (ItemType)(int)Random.Range(0, 3), (ItemGrade)(int)Random.Range(0, 4)));
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
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
