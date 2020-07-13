using System.Collections;
using System.Collections.Generic;

using System;
using UnityEngine;

public class Cleaner : MonoBehaviour
{
    Action onClean;

    void Start()
    {
        onClean += CleaningRoomA;
        onClean += CleaningRoomB;
    }

    void Update()
    {
        if(Input.GetMouseButtonDown(0))
        {
            onClean();
        }
    }

    void CleaningRoomA()
    {
        Debug.Log("A방 청소");
    }

    void CleaningRoomB()
    {
        Debug.Log("B방 청소");
    }
}

public class Monster : MonoBehaviour
{
    public virtual void Attack()
    {
        Debug.Log("공격!");
        //공격 처리...
    }
}

public class Orc : Monster
{
    public override void Attack()
    {
        base.Attack();
        Debug.Log("우리는 노예가 되지 않는다.");
    }
}

public class Dragon : Monster
{
    public override void Attack()
    {
        base.Attack();
        Debug.Log("모든 것이 불타오를 것이다!");
    }
}

public class Main : MonoBehaviour
{
    private void Start()
    {
        Orc orc = FindObjectOfType<Orc>();
        Monster monster = orc;
        monster.Attack(); // Orc의 Attack()이 실행됨
    }
}

public class Player : MonoBehaviour
{
    public GameData gameData;

    public void Die()
    {
        // 실제 사망 처리...
        gameData.Save();
    }
}

public class GameData : MonoBehaviour
{
    public void Save()
    {
        Debug.Log("게임 저장...");
    }
}

