using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class ItemData{
    public int id;
    public string name;
    public ItemSystem.ItemType itemType;
    public ItemSystem.ItemGrade itemGrade;
    public string itemInfo;

    public ItemData(int id, string name, ItemSystem.ItemType itemType, ItemSystem.ItemGrade itemGrade, string itemInfo)
    {
        this.id = id;
        this.name = name;
        this.itemType = itemType;
        this.itemGrade = itemGrade;
        this.itemInfo = itemInfo;
    }
}

[System.Serializable]
public class WeaponData : ItemData
{
    public int attack;

    public WeaponData(int id, string name, ItemSystem.ItemType itemType, ItemSystem.ItemGrade itemGrade, string itemInfo, int attack): base(id,name,itemType, itemGrade,itemInfo)
    {
        this.attack = attack;
    }
}

[System.Serializable]
public class ConsumeData : ItemData
{
    public int point;

    public ConsumeData(int id, string name, ItemSystem.ItemType itemType, ItemSystem.ItemGrade itemGrade, string itemInfo, int point) : base(id, name, itemType, itemGrade, itemInfo)
    {
        this.point = point;
    }
}

[System.Serializable]
public class ArmorData : ItemData
{
    public int defense;

    public ArmorData(int id, string name, ItemSystem.ItemType itemType, ItemSystem.ItemGrade itemGrade, string itemInfo, int defense) : base(id, name, itemType, itemGrade, itemInfo)
    {
        this.defense = defense;
    }
}