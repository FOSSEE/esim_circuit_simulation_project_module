<?php
function verify_certificates($qr_code = 0)
{
	$qr_code = arg(3);
	$page_content = "";
	if ($qr_code)
	{
		$page_content = verify_qrcode_fromdb($qr_code);
	} //$qr_code
	else
	{
		$verify_certificates_form = drupal_get_form("verify_certificates_form");
		$page_content = drupal_render($verify_certificates_form);
	}
	return $page_content;
}
function verify_certificates_form($form, &$form_state)
{
	$form = array();
	$form['Title'] = array(
		'#type' => 'markup',
		'#markup' => ''
	);
	$form["QR_code"] = array(
		"#type" => "textfield",
		"#title" => "Enter QR Code",
		"#default_value" => '',
		"#required" => TRUE
	);
	$form["submit"] = array(
		"#type" => "submit",
		"#value" => "Verify",
		'#ajax' => array(
			'callback' => 'verify_certificates_form_submit',
			'progress' => array(
				'message' => ''
			)
		)
	);
	$form['displaytable'] = array(
		'#type' => 'markup',
		'#prefix' => '<div><div id="displaytable" style="font-weight:bold;padding-top:10px">',
		'#suffix' => '</div></div>',
		'#markup' => ''
	);
	return $form;
}
function verify_certificates_form_submit($form, &$form_state)
{
	$page_content = "";
	$v = $form_state["values"];
	$qr_code = $v["QR_code"];
	$page_content = verify_qrcode_fromdb($qr_code);
	$form['displaytable']['#markup'] = $page_content;
	$commands[] = ajax_command_html("#displaytable", drupal_render($form['displaytable']));
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
function verify_qrcode_fromdb($qr_code)
{
	$query = db_select('dwsim_flowsheet_qr_code');
	$query->fields('dwsim_flowsheet_qr_code', array(
		'proposal_id'
	));
	$query->condition('qr_code', $qr_code);
	$result = $query->execute();
	$proposal_id = $result->fetchObject()->proposal_id;
	if ($proposal_id)
	{
		$query2 = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE approval_status=3", array(
		':uid' => $proposal_id
		));
		$data2 = $query2->fetchObject();
		$query3 = db_query("SELECT * FROM dwsim_flowsheet_proposal WHERE approval_status=3 AND id=:uid", array(
		':uid' => $proposal_id
	));
	$data3 = $query3->fetchObject();
		/*$query3 = db_query("SELECT * FROM {dwsim_flowsheet_proposal} WHERE approval_status=3 AND uid= :uid", array(
		':uid' => $user->uid
		));
		$data3 = $query3->fetchObject();**/
		$page_content = "";
		$page_content .= "<h4>Participation Details</h4><table><tr><td>Name</td>";
		$page_content .= "<td>" . $data3->contributor_name . "</td></tr>";
		$page_content .= "<tr><td>Project</td>";
		$page_content .= "<td>DWSIM Flowsheeting Project</td></tr>";
		$page_content .= "<tr><td>Flowsheets completed</td>";
		$page_content .= "<td>" . $data3->project_title . "</td></tr>";
		//$page_content .= "<tr><td>Book Author</td>";
		//$page_content .= "<td>" . $data2->author . "</td></tr>";
		$page_content .= "</table>";
	} //$proposal_id
	else
	{
		$page_content = "<b>Sorry ! The serial number you entered seems to be invalid. Please try again ! <b>";
	}
	return $page_content;
}
