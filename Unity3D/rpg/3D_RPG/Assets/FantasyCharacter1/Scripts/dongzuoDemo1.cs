using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class dongzuoDemo1 : MonoBehaviour {

    public GameObject attackBullet;
    public GameObject magicBullet;
    public GameObject magic2Bullet;
    public GameObject ultimateBullet;
    public GameObject damageEffect1;
    public GameObject damageEffect2;
    public GameObject damageEffect3;
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    float padding = 2.5f;
    List<PosBullet1> allList = new List<PosBullet1>();
    IEnumerator delayBullet()
    {
        int count = 3;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(attackBullet);
            PosBullet1 bullet = obj.GetComponent<PosBullet1>();
            bullet.player = transform;
            bullet.effectObj = damageEffect1;
            Vector3 startPos = transform.position + new Vector3(Random.Range(-padding, padding), Random.Range(1f, 2f), Random.Range(-padding, padding));
            bullet.startPos = startPos;
            bullet.transform.position = startPos;
            allList.Add(bullet);
            yield return new WaitForSeconds(0.1f);
        }
    }
    bool flag = false;
    IEnumerator delayBullet1()
    {
        int count = 6;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(attackBullet);
            PosBullet1 bullet = obj.GetComponent<PosBullet1>();
            bullet.player = transform;
            bullet.effectObj = damageEffect1;
            Vector3 startPos = transform.position + new Vector3(Random.Range(-padding, padding), Random.Range(1f, 3f), Random.Range(-padding, padding));
            bullet.startPos = startPos;
            bullet.transform.position = startPos;
            allList.Add(bullet);
            yield return new WaitForSeconds(0.1f);
        }
        yield return new WaitForSeconds(1f);
        if(flag)
        {
            yield break;
        }
        flag = true;
        StartCoroutine(delayBullet3());
    }

    IEnumerator delayBullet2()
    {
        int count = 9;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(attackBullet);
            PosBullet1 bullet = obj.GetComponent<PosBullet1>();
            bullet.player = transform;
            bullet.effectObj = damageEffect1;
            Vector3 startPos = transform.position + new Vector3(Random.Range(-padding, padding), Random.Range(1f, 3f), Random.Range(-padding, padding));
            bullet.startPos = startPos;
            bullet.transform.position = startPos;
            allList.Add(bullet);
            yield return new WaitForSeconds(0.1f);
        }
        
    }
    float yoff = 1.5f;
    IEnumerator delayBullet3()
    {
        GameObject obj = GameObject.Instantiate(magicBullet);
        obj.transform.position = transform.position + new Vector3(0f, yoff, 0f);
        ParticleScaler1 scale = obj.GetComponent<ParticleScaler1>();
        for (int i = 0; i < allList.Count; i++)
        {
            PosBullet1 bullet = allList[i];
            if(bullet != null && bullet.gameObject != null)
            {
                bullet.tarPos = transform.position + new Vector3(0f, yoff, 0f);
                bullet.bulleting();
                yield return new WaitForSeconds(0.01f);
                scale.particleScale += 0.15f;
            }
           
        }
        yield return new WaitForSeconds(1f);
        PosBullet1 b = obj.GetComponent<PosBullet1>();
        AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
        Vector3 attackedPos = MathUtil1.findChild(c.transform, "attackedPivot").position;
        b.tarPos = attackedPos;
        b.player = transform;
        b.target = c.transform;
        b.startPos = b.transform.position;
        b.effectObj = damageEffect2;
        b.bulleting();
        allList.Clear();
        flag = false;
    }

    void preAction(string actionName)
    {
        AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
        string[] arr = actionName.Split('|');
        string name = arr[0];
        switch(name)
        {
            case AnimationName1.Attack:
                if (damageEffect1 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect1);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
                c.attacked();
                StartCoroutine(delayBullet());
                break;
            case AnimationName1.Magic:
               
                if (damageEffect3 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect3);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
                c.attacked();
                StartCoroutine(delayBullet1());
                break;
            case AnimationName1.Magic2:
                
                if (damageEffect2 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect2);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();

                    effect.transform.position = GameObject.Find("bigzhangjiao (1)").transform.position;
                    effect.play();
                }
                c.attacked();
                StartCoroutine(delayBullet2());
                break;
            case AnimationName1.Ultimate:

                if(damageEffect3 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect3);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();
                    effect.time = 2f;
                    effect.transform.position = GameObject.Find("bigzhangjiao (1)").transform.position;
                    effect.play();
                }
                c.attacked();
                
                break;
        }
    }

    IEnumerator delayAttacked()
    {
        yield return new WaitForSeconds(1.5f);
        AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
        c.attacked();
        //yield return new WaitForSeconds(2.5f);
        //c.attacked();
    }
}
