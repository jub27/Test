using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Character
{
    private int gold;
    public int Gold{ get { return gold; } }

    private int stage;
    public int Stage{ get { return stage; } }

    private int hitPoint;
    public int HitPoint { get { return hitPoint; } }

    private int shield;
    public int Shield { get { return shield; } }

    private int ammo;
    public int Ammo { get { return ammo; } }

    public Character()
    {
        gold = 0;
        stage = 1;
        hitPoint = 100;
        shield = 100;
        ammo = 100;
    }

    public void SetCharacter(int _gold, int _stage, int _hitPoint, int _shield, int _ammo)
    {
        gold        = _gold;
        stage       = _stage;
        hitPoint    = _hitPoint;
        shield      = _shield;
        ammo        = _ammo;
    }
}

public class Player
{
    private string nickName;
    public string NickName { get { return nickName; } }

    private int passward;
    public int PassWard { get { return passward; } }

    private List<Character> characterList;
    public List<Character> CharacterList { get { return characterList; } }

    public Player(string _nickName, int _passward, List<Character> _characters)
    {
        nickName        = _nickName;
        passward        = _passward;
        characterList   = _characters;
    }

    public void AddCharacter()
    {
        characterList.Add(new Character());
    }

    public void UpdateCharacter(int index, int gold, int stage, int hitpoint, int shield, int ammo)
    {
        characterList[index].SetCharacter(gold, stage, hitpoint, shield, ammo);
    }
}
