using UnityEngine;

public class CameraController : MonoBehaviour
{
	public GameObject target;
	public float smoothSpeed = 0.125f;
	public Vector3 offset;

	void LateUpdate()
	{
		Vector3 desiredPosition = target.transform.position + offset;
		Vector3 smoothedPosition = Vector3.Lerp(transform.position, desiredPosition, smoothSpeed);
		transform.position = smoothedPosition;

		transform.LookAt(target.transform);
	}
}