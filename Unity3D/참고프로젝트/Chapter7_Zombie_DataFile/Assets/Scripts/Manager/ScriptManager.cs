using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScriptManager
{
    private static ScriptManager m_pThis = null;
    public static ScriptManager GetInstance
    {
        get
        {
            if(m_pThis == null)
            {
                m_pThis = new ScriptManager();
                m_pThis.Init();
            }

            return m_pThis;
        }
    }

    private Dictionary<int, ItemData> _ItemDataDic = new Dictionary<int, ItemData>();
    public Dictionary<int, ItemData> ItemDataDic
    {
        get { return _ItemDataDic; }
    }

    void Init()
    {
        ItemDataLoader ItemDataLoader = new ItemDataLoader();
        _ItemDataDic = ItemDataLoader.GetDic();
    }
}
