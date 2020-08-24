using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class RotateBullet1:Bullet1
{
	protected override void init ()
	{
		base.init ();
	
	}

    float h = 0f;
	public override void bulleting ()
	{
		base.bulleting ();
        h = y;
        Vector3 attackPos = player.transform.position + new Vector3(0f, h, 0f);
		transform.position = attackPos;
		stopParticle ();
		StartCoroutine (delayBulleting ());
	}

    public float flag = 1f;
	IEnumerator delayBulleting()
	{
		yield return null;
		bulletState = BulletState1.line;
		Vector3 attackPos = player.transform.position + new Vector3(0f, h, 0f);
        transform.position = attackPos;
		startParticle ();
	}

	protected override void complete ()
	{
		base.complete ();
	}
    float time = 0f;
    public float r = 0f;
    public float y;
    public Vector3 tarPos;
	protected override void update ()
	{
		if(bulletState == BulletState1.none)
		{
			return;
		}

        if(bulletState == BulletState1.wand)
        {
            
            Vector3 attackedPos = tarPos;
            speed = 10f;
            if (Vector3.SqrMagnitude(attackedPos - transform.position) <= speed * Time.deltaTime)
            {
                complete();
                return;
            }

            Vector3 pos1 = MathUtil1.calcTargetPosByDis(transform.position, attackedPos, speed * Time.deltaTime);
            transform.forward = pos1 - transform.position;
            transform.position = pos1;
            return;
        }

        time += Time.deltaTime;
        //r += Time.deltaTime * speed;
        if(time >= 7f)
        {
            complete();
            return;
        }
        if(player == null)
        {
            return;
        }
        Vector3 pos = MathUtil1.calcTargetPosByRotation(player, 90f * time * flag, r, false);
        pos.y += y;
		base.update ();
        transform.forward = pos - transform.position;
        transform.position = pos;
        
		
	}
}
