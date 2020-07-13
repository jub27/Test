using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VolumeInfo : MonoBehaviour
{
    public float megaBytes
    {
        get { return m_bytes * 0.00001f; }

        set
        {
            if (value < 0)
            {
                m_bytes = 0;
            }
            else
            {
                m_bytes = value * 1000000f;
            }
        }
    }

    public float kiloBytes
    {
        get { return m_bytes * 0.001f; }

        set
        {
            if(value <= 0)
            {
                m_bytes = 0;
            }
            else
            {
                m_bytes = value * 1000f;
            }
        }
    }

    private float m_bytes;
    public float bytes
    {
        get { return m_bytes; }

        set
        {
            if (value <= 0)
                m_bytes = 0;
            else
            {
                m_bytes = value;
            }
        }
    }
}
