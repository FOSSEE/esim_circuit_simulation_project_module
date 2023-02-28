<?php
// $Id$
function circuit_simulation_proposal_pending()
{
	/* get pending proposals to be approved */
	$pending_rows = array();
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('approval_status', 0);
	$query->orderBy('id', 'DESC');
	$pending_q = $query->execute();
	while ($pending_data = $pending_q->fetchObject())
	{
		$pending_rows[$pending_data->id] = array(
			date('d-m-Y', $pending_data->creation_date),
			l($pending_data->name_title . ' ' . $pending_data->contributor_name, 'user/' . $pending_data->uid),
			$pending_data->project_title,
			l('Approve', 'circuit-simulation-project/manage-proposal/approve/' . $pending_data->id) . ' | ' . l('Edit', 'circuit-simulation-project/manage-proposal/edit/' . $pending_data->id)
		);
	} //$pending_data = $pending_q->fetchObject()
	/* check if there are any pending proposals */
	if (!$pending_rows)
	{
		drupal_set_message(t('There are no pending proposals.'), 'status');
		return '';
	} //!$pending_rows
	$pending_header = array(
		'Date of Submission',
		'Student Name',
		'Title of the Circuit Simulation Project',
		'Action'
	);
	//$output = theme_table($pending_header, $pending_rows);
	$output = theme('table', array(
		'header' => $pending_header,
		'rows' => $pending_rows
	));
	return $output;
}
function circuit_simulation_proposal_all()
{
	/* get pending proposals to be approved */
	$proposal_rows = array();
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->orderBy('id', 'DESC');
	$proposal_q = $query->execute();
	while ($proposal_data = $proposal_q->fetchObject())
	{
		$approval_status = '';
		switch ($proposal_data->approval_status)
		{
			case 0:
				$approval_status = 'Pending';
				break;
			case 1:
				$approval_status = 'Approved';
				break;
			case 2:
				$approval_status = 'Dis-approved';
				break;
			case 3:
				$approval_status = 'Completed';
				break;
			default:
				$approval_status = 'Unknown';
				break;
		} //$proposal_data->approval_status
		if ($proposal_data->actual_completion_date == 0)
		{
			$actual_completion_date = "Not Completed";
		} //$proposal_data->actual_completion_date == 0
		else
		{
			$actual_completion_date = date('d-m-Y', $proposal_data->actual_completion_date);
		}
		$proposal_rows[] = array(
			date('d-m-Y', $proposal_data->creation_date),
			l($proposal_data->contributor_name, 'user/' . $proposal_data->uid),
			$proposal_data->project_title,
			$actual_completion_date,
			$approval_status,
			l('Status', 'circuit-simulation-project/manage-proposal/status/' . $proposal_data->id) . ' | ' . l('Edit', 'circuit-simulation-project/manage-proposal/edit/' . $proposal_data->id)
		);
	} //$proposal_data = $proposal_q->fetchObject()
	/* check if there are any pending proposals */
	if (!$proposal_rows)
	{
		drupal_set_message(t('There are no proposals.'), 'status');
		return '';
	} //!$proposal_rows
	$proposal_header = array(
		'Date of Submission',
		'Student Name',
		'Title of the circuit-simulation project',
		'Date of Completion',
		'Status',
		'Action'
	);
	$output = theme('table', array(
		'header' => $proposal_header,
		'rows' => $proposal_rows
	));
	return $output;
}
/******************************************************************************/
/************************** PROPOSAL APPROVAL FORM ****************************/
/******************************************************************************/
function circuit_simulation_proposal_approval_form($form, &$form_state)
{
	global $user;
	/* get current proposal */
	$proposal_id = (int) arg(3);
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
			drupal_goto('circuit-simulation-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('circuit-simulation-project/manage-proposal');
		return;
	}
	if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
	{
		$project_guide_name = "Not Entered";
	} //$proposal_data->project_guide_name == NULL
	else
	{
		$project_guide_name = $proposal_data->project_guide_name;
	}
	if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
	{
		$project_guide_email_id = "Not Entered";
	} //$proposal_data->project_guide_email_id == NULL
	else
	{
		$project_guide_email_id = $proposal_data->project_guide_email_id;
	}
	$form['contributor_name'] = array(
		'#type' => 'item',
		'#markup' => l($proposal_data->name_title . ' ' . $proposal_data->contributor_name, 'user/' . $proposal_data->uid),
		'#title' => t('Student name')
	);
	$form['student_email_id'] = array(
		'#title' => t('Student Email'),
		'#type' => 'item',
		'#markup' => user_load($proposal_data->uid)->mail,
		'#title' => t('Email')
	);
	$form['contributor_contact_no'] = array(
		'#title' => t('Contact No.'),
		'#type' => 'item',
		'#markup' => $proposal_data->contact_no,
	);
	$form['university'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->university,
		'#title' => t('University/Institute')
	);
	$form['country'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->country,
		'#title' => t('Country')
	);
	$form['all_state'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->state,
		'#title' => t('State')
	);
	$form['city'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->city,
		'#title' => t('City')
	);
	$form['pincode'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->pincode,
		'#title' => t('Pincode/Postal code')
	);
	$form['project_guide_name'] = array(
		'#type' => 'item',
		'#title' => t('Project guide'),
		'#markup' => $project_guide_name
	);
	$form['project_guide_email_id'] = array(
		'#type' => 'item',
		'#title' => t('Project guide email'),
		'#markup' => $project_guide_email_id
	);
	$form['operating_system'] = array(
		'#type' => 'item',
		'#title' => t('Operating System'),
		'#markup' => $proposal_data->operating_system,
	);
	$form['project_title'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->project_title,
		'#title' => t('Title of the Circuit Simulation Project')
	);
	$form['description'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->description,
		'#title' => t('Description of the Circuit Simulation Project')
	);
	if (($proposal_data->samplefilepath != "") && ($proposal_data->samplefilepath != 'NULL'))
	{
		$str = substr($proposal_data->samplefilepath,strrpos($proposal_data->samplefilepath, '/'));
		$resource_file =ltrim($str, '/');
		
		$form['samplefilepath'] = array(
			'#type' => 'item',
			'#title' => t('Resource file '),
			'#markup' => l($resource_file, 'circuit-simulation-project/download/resource-file/' . $proposal_id) . ""
		);
	} //$proposal_data->user_defined_compound_filepath != ""
	else
	{
		$form['samplefilepath'] = array(
			'#type' => 'item',
			'#title' => t('Resource file '),
			'#markup' => "Not uploaded<br><br>"
		);
	}
	$form['approval'] = array(
		'#type' => 'radios',
		'#title' => t('eSim circuit-simulation proposal'),
		'#options' => array(
			'1' => 'Approve',
			'2' => 'Disapprove'
		),
		'#required' => TRUE
	);
	$form['message'] = array(
		'#type' => 'textarea',
		'#title' => t('Reason for disapproval'),
		'#attributes' => array(
			'placeholder' => t('Enter reason for disapproval in minimum 30 characters '),
			'cols' => 50,
			'rows' => 4
		),
		'#states' => array(
			'visible' => array(
				':input[name="approval"]' => array(
					'value' => '2'
				)
			)
		)
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	$form['cancel'] = array(
		'#type' => 'item',
		'#markup' => l(t('Cancel'), 'circuit-simulation-project/manage-proposal')
	);
	return $form;
}
function circuit_simulation_proposal_approval_form_validate($form, &$form_state)
{
	if ($form_state['values']['approval'] == 2)
	{
		if ($form_state['values']['message'] == '')
		{
			form_set_error('message', t('Reason for disapproval could not be empty'));
		} //$form_state['values']['message'] == ''
	} //$form_state['values']['approval'] == 2
}
function circuit_simulation_proposal_approval_form_submit($form, &$form_state)
{
	global $user;
	/* get current proposal */
	$proposal_id = (int) arg(3);
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
			drupal_goto('circuit-simulation-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('circuit-simulation-project/manage-proposal');
		return;
	}
	if ($form_state['values']['approval'] == 1)
	{
		$query = "UPDATE {esim_circuit_simulation_proposal} SET approver_uid = :uid, approval_date = :date, approval_status = 1 WHERE id = :proposal_id";
		$args = array(
			":uid" => $user->uid,
			":date" => time(),
			":proposal_id" => $proposal_id
		);
		db_query($query, $args);
		/* sending email */
		$user_data = user_load($proposal_data->uid);
		$email_to = $user_data->mail;
		$from = variable_get('circuit_simulation_from_email', '');
		$bcc = $user->mail . ', ' . variable_get('circuit_simulation_emails', '');
		$cc = variable_get('circuit_simulation_cc_emails', '');
		$params['circuit_simulation_proposal_approved']['proposal_id'] = $proposal_id;
		$params['circuit_simulation_proposal_approved']['user_id'] = $proposal_data->uid;
		$params['circuit_simulation_proposal_approved']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('circuit_simulation', 'circuit_simulation_proposal_approved', $email_to, language_default(), $params, $from, TRUE))
			drupal_set_message('Error sending email message.', 'error');
		drupal_set_message('eSim circuit-simulation proposal No. ' . $proposal_id . ' approved. User has been notified of the approval.', 'status');
		drupal_goto('circuit-simulation-project/manage-proposal');
		return;
	} //$form_state['values']['approval'] == 1
	else if ($form_state['values']['approval'] == 2)
	{
		$query = "UPDATE {esim_circuit_simulation_proposal} SET approver_uid = :uid, approval_date = :date, approval_status = 2, dissapproval_reason = :dissapproval_reason WHERE id = :proposal_id";
		$args = array(
			":uid" => $user->uid,
			":date" => time(),
			":dissapproval_reason" => $form_state['values']['message'],
			":proposal_id" => $proposal_id
		);
		$result = db_query($query, $args);
		/* sending email */
		$user_data = user_load($proposal_data->uid);
		$email_to = $user_data->mail;
		$from = variable_get('circuit_simulation_from_email', '');
		$bcc = $user->mail . ', ' . variable_get('circuit_simulation_emails', '');
		$cc = variable_get('circuit_simulation_cc_emails', '');
		$params['circuit_simulation_proposal_disapproved']['proposal_id'] = $proposal_id;
		$params['circuit_simulation_proposal_disapproved']['user_id'] = $proposal_data->uid;
		$params['circuit_simulation_proposal_disapproved']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('circuit_simulation', 'circuit_simulation_proposal_disapproved', $email_to, language_default(), $params, $from, TRUE))
			drupal_set_message('Error sending email message.', 'error');
		drupal_set_message('eSim circuit simulation proposal No. ' . $proposal_id . ' dis-approved. User has been notified of the dis-approval.', 'error');
		drupal_goto('circuit-simulation-project/manage-proposal');
		return;
	} //$form_state['values']['approval'] == 2
}
/******************************************************************************/
/*************************** PROPOSAL STATUS FORM *****************************/
/******************************************************************************/
function circuit_simulation_proposal_status_form($form, &$form_state)
{
	global $user;
	/* get current proposal */
	$proposal_id = (int) arg(3);
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
			drupal_goto('circuit-simulation-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('circuit-simulation-project/manage-proposal');
		return;
	}
	$form['contributor_name'] = array(
		'#type' => 'item',
		'#markup' => l($proposal_data->name_title . ' ' . $proposal_data->contributor_name, 'user/' . $proposal_data->uid),
		'#title' => t('Student name')
	);
	$form['student_email_id'] = array(
		'#title' => t('Student Email'),
		'#type' => 'item',
		'#markup' => user_load($proposal_data->uid)->mail,
		'#title' => t('Email')
	);
	$form['university'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->university,
		'#title' => t('University/Institute')
	);
	$form['country'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->country,
		'#title' => t('Country')
	);
	$form['all_state'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->state,
		'#title' => t('State')
	);
	$form['city'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->city,
		'#title' => t('City')
	);
	$form['pincode'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->pincode,
		'#title' => t('Pincode/Postal code')
	);
	$form['project_guide_name'] = array(
		'#type' => 'item',
		'#title' => t('Project guide'),
		'#markup' => $proposal_data->project_guide_name
	);
	$form['project_guide_email_id'] = array(
		'#type' => 'item',
		'#title' => t('Project guide email'),
		'#markup' => $proposal_data->project_guide_email_id
	);
	$form['operating_system'] = array(
		'#type' => 'item',
		'#title' => t('Operating System'),
		'#markup' => $proposal_data->operating_system
	);
	$form['project_title'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->project_title,
		'#title' => t('Title of the Circuit Simulation Project')
	);
	$form['description'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->description,
		'#title' => t('Description of the Circuit Simulation Project')
	);
	/************************** reference link filter *******************/
	$url = '~(?:(https?)://([^\s<]+)|(www\.[^\s<]+?\.[^\s<]+))(?<![\.,:])~i';
	$reference = preg_replace($url, '<a href="$0" target="_blank" title="$0">$0</a>', $proposal_data->reference);
	/******************************/
	$form['reference'] = array(
		'#type' => 'item',
		'#markup' => $reference,
		'#title' => t('References')
	);
	$proposal_status = '';
	switch ($proposal_data->approval_status)
	{
		case 0:
			$proposal_status = t('Pending');
			break;
		case 1:
			$proposal_status = t('Approved');
			break;
		case 2:
			$proposal_status = t('Dis-approved');
			break;
		case 3:
			$proposal_status = t('Completed');
			break;
		default:
			$proposal_status = t('Unkown');
			break;
	} 
	$form['proposal_status'] = array(
		'#type' => 'item',
		'#markup' => $proposal_status,
		'#title' => t('Proposal Status')
	);
	if ($proposal_data->approval_status == 0)
	{
		$form['approve'] = array(
			'#type' => 'item',
			'#markup' => l('Click here', 'circuit-simulation-project/manage-proposal/approve/' . $proposal_id),
			'#title' => t('Approve')
		);
	} //$proposal_data->approval_status == 0
	if ($proposal_data->approval_status == 1)
	{
		$form['completed'] = array(
			'#type' => 'checkbox',
			'#title' => t('Completed'),
			'#description' => t('Check if user has provided all the required files and pdfs.')
		);
	} //$proposal_data->approval_status == 1
	if ($proposal_data->approval_status == 2)
	{
		$form['message'] = array(
			'#type' => 'item',
			'#markup' => $proposal_data->message,
			'#title' => t('Reason for disapproval')
		);
	} //$proposal_data->approval_status == 2
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	$form['cancel'] = array(
		'#type' => 'markup',
		'#markup' => l(t('Cancel'), 'circuit-simulation-project/manage-proposal/all')
	);
	return $form;
}
function circuit_simulation_proposal_status_form_submit($form, &$form_state)
{
	global $user;
	/* get current proposal */
	$proposal_id = (int) arg(3);
	//$proposal_q = db_query("SELECT * FROM {esim_circuit_simulation_proposal} WHERE id = %d", $proposal_id);
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
			drupal_goto('circuit-simulation-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('circuit-simulation-project/manage-proposal');
		return;
	}
	/* set the book status to completed */
	if ($form_state['values']['completed'] == 1)
	{
		$up_query = "UPDATE esim_circuit_simulation_proposal SET approval_status = :approval_status , actual_completion_date = :expected_completion_date WHERE id = :proposal_id";
		$args = array(
			":approval_status" => '3',
			":proposal_id" => $proposal_id,
			":expected_completion_date" => time()
		);
		$result = db_query($up_query, $args);
		CreateReadmeFileeSimCircuitSimulationProject($proposal_id);
		if (!$result)
		{
			drupal_set_message('Error in update status', 'error');
			return;
		} //!$result
		/* sending email */
		$user_data = user_load($proposal_data->uid);
		$email_to = $user_data->mail;
		$from = variable_get('circuit_simulation_from_email', '');
		$bcc = $user->mail . ', ' . variable_get('circuit_simulation_emails', '');
		$cc = variable_get('circuit_simulation_cc_emails', '');
		$params['circuit_simulation_proposal_completed']['proposal_id'] = $proposal_id;
		$params['circuit_simulation_proposal_completed']['user_id'] = $proposal_data->uid;
		$params['circuit_simulation_proposal_completed']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('circuit_simulation', 'circuit_simulation_proposal_completed', $email_to, language_default(), $params, $from, TRUE))
			drupal_set_message('Error sending email message.', 'error');
		drupal_set_message('Congratulations! eSim circuit simulation proposal has been marked as completed. User has been notified of the completion.', 'status');
	}
		drupal_goto('circuit-simulation-project/manage-proposal');
		return;
	
}
/******************************************************************************/
/**************************** PROPOSAL EDIT FORM ******************************/
/******************************************************************************/
function circuit_simulation_proposal_edit_form($form, &$form_state)
{
	global $user;
	/* get current proposal */
	$proposal_id = (int) arg(3);
	//$proposal_q = db_query("SELECT * FROM {esim_circuit_simulation_proposal} WHERE id = %d", $proposal_id);
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
			drupal_goto('circuit-simulation-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('circuit-simulation-project/manage-proposal');
		return;
	}
	$user_data = user_load($proposal_data->uid);
	$form['name_title'] = array(
		'#type' => 'select',
		'#title' => t('Title'),
		'#options' => array(
			'Dr' => 'Dr',
			'Prof' => 'Prof',
			'Mr' => 'Mr',
			'Mrs' => 'Mrs',
			'Ms' => 'Ms'
		),
		'#required' => TRUE,
		'#default_value' => $proposal_data->name_title
	);
	$form['contributor_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the Proposer'),
		'#size' => 30,
		'#maxlength' => 50,
		'#required' => TRUE,
		'#default_value' => $proposal_data->contributor_name
	);
	$form['student_email_id'] = array(
		'#type' => 'item',
		'#title' => t('Email'),
		'#markup' => $user_data->mail
	);
	$form['university'] = array(
		'#type' => 'textfield',
		'#title' => t('University/Institute'),
		'#size' => 200,
		'#maxlength' => 200,
		'#required' => TRUE,
		'#default_value' => $proposal_data->university
	);
	$form['project_guide_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Project guide'),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter full name of project guide')
		),
		'#maxlength' => 250,
		'#default_value' => $proposal_data->project_guide_name
	);
	$form['project_guide_email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Project guide email'),
		'#size' => 30,
		'#default_value' => $proposal_data->project_guide_email_id
	);
	$form['country'] = array(
		'#type' => 'select',
		'#title' => t('Country'),
		'#options' => array(
			'India' => 'India',
			'Others' => 'Others'
		),
		'#default_value' => $proposal_data->country,
		'#required' => TRUE,
		'#tree' => TRUE,
		'#validated' => TRUE
	);
	$form['other_country'] = array(
		'#type' => 'textfield',
		'#title' => t('Other than India'),
		'#size' => 100,
		'#default_value' => $proposal_data->country,
		'#attributes' => array(
			'placeholder' => t('Enter your country name')
		),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['other_state'] = array(
		'#type' => 'textfield',
		'#title' => t('State other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your state/region name')
		),
		'#default_value' => $proposal_data->state,
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['other_city'] = array(
		'#type' => 'textfield',
		'#title' => t('City other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your city name')
		),
		'#default_value' => $proposal_data->city,
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['all_state'] = array(
		'#type' => 'select',
		'#title' => t('State'),
		'#options' => _df_list_of_states(),
		'#default_value' => $proposal_data->state,
		'#validated' => TRUE,
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'India'
				)
			)
		)
	);
	$form['city'] = array(
		'#type' => 'select',
		'#title' => t('City'),
		'#options' => _df_list_of_cities(),
		'#default_value' => $proposal_data->city,
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'India'
				)
			)
		)
	);
	$form['pincode'] = array(
		'#type' => 'textfield',
		'#title' => t('Pincode'),
		'#size' => 30,
		'#maxlength' => 6,
		'#default_value' => $proposal_data->pincode,
		'#attributes' => array(
			'placeholder' => 'Insert pincode of your city/ village....'
		)
	);
	$form['operating_system'] = array(
		'#type' => 'select',
		'#title' => t('Operating System'),
		'#options' => array(
			'Ubuntu' => 'Ubuntu',
			'Windows' => 'Windows'
		),
		'#required' => TRUE,
		'#default_value' => $proposal_data->operating_system
	);
	$form['project_title'] = array(
		'#type' => 'textarea',
		'#title' => t('Title of the Circuit Simulation Project'),
		'#size' => 300,
		'#maxlength' => 350,
		'#required' => TRUE,
		'#default_value' => $proposal_data->project_title
	);
	$form['description'] = array(
		'#type' => 'textarea',
		'#title' => t('Description'),
		'#description' => t('Minimum character limit is 500 and Maximum character limit is 700'),
		'#required' => TRUE,
		'#default_value' => $proposal_data->description
	);	
	$form['reference'] = array(
		'#type' => 'textarea',
		'#title' => t('Reference'),		
		'#size' => 250,
		'#maxlength' => 1000,
		'#attributes' => array(
			'placeholder' => 'Links of must be provided....'
		),
		'#default_value' => $proposal_data->reference
	);
	$form['delete_proposal'] = array(
		'#type' => 'checkbox',
		'#title' => t('Delete Proposal')
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	$form['cancel'] = array(
		'#type' => 'item',
		'#markup' => l(t('Cancel'), 'circuit-simulation-project/manage-proposal')
	);
	return $form;
}

function circuit_simulation_proposal_form_validate($form, &$form_state)
{
	//Validation for project title
	$form_state['values']['project_title'] = trim($form_state['values']['project_title']);
	if ($form_state['values']['project_title'] != '')
	{
		if (strlen($form_state['values']['project_title']) > 250)
		{
			form_set_error('project_title', t('Maximum charater limit is 250 charaters only, please check the length of the project title'));
		} //strlen($form_state['values']['project_title']) > 250
		else if (strlen($form_state['values']['project_title']) < 10)
		{
			form_set_error('project_title', t('Minimum charater limit is 10 charaters, please check the length of the project title'));
		} //strlen($form_state['values']['project_title']) < 10
	} //$form_state['values']['project_title'] != ''
	else
	{
		form_set_error('project_title', t('Project title shoud not be empty'));
	}

	$form_state['values']['description'] = trim($form_state['values']['description']);
	if ($form_state['values']['description'] != '')
	{
		if (strlen($form_state['values']['description']) > 700)
		{
			form_set_error('description', t('Maximum charater limit is 700 charaters only, please check the length of the description'));
		} //strlen($form_state['values']['project_title']) > 250
		else if (strlen($form_state['values']['description']) < 500)
		{
			form_set_error('description', t('Minimum charater limit is 500 charaters, please check the length of the description'));
		} //strlen($form_state['values']['project_title']) < 10
	} //$form_state['values']['project_title'] != ''
	else
	{
		form_set_error('description', t('Description shoud not be empty'));
	}
	return $form_state;
}

function circuit_simulation_proposal_edit_form_submit($form, &$form_state)
{
	global $user;
	/* get current proposal */
	$proposal_id = (int) arg(3);
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
			drupal_goto('circuit-simulation-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
		drupal_goto('circuit-simulation-project/manage-proposal');
		return;
	}
	/* delete proposal */
	if ($form_state['values']['delete_proposal'] == 1)
	{
		/* sending email */
		$user_data = user_load($proposal_data->uid);
		$email_to = $user_data->mail;
		$from = variable_get('circuit_simulation_from_email', '');
		$bcc = variable_get('circuit_simulation_emails', '');
		$cc = variable_get('circuit_simulation_cc_emails', '');
		$params['circuit_simulation_proposal_deleted']['proposal_id'] = $proposal_id;
		$params['circuit_simulation_proposal_deleted']['user_id'] = $proposal_data->uid;
		$params['circuit_simulation_proposal_deleted']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('circuit_simulation', 'circuit_simulation_proposal_deleted', $email_to, user_preferred_language($user), $params, $from, TRUE))
			drupal_set_message('Error sending email message.', 'error');
		drupal_set_message(t('eSim Cicuit simulation proposal has been deleted.'), 'status');
		if (rrmdir_project($proposal_id) == TRUE)
		{
			$query = db_delete('esim_circuit_simulation_proposal');
			$query->condition('id', $proposal_id);
			$num_deleted = $query->execute();
			drupal_set_message(t('Proposal Deleted'), 'status');
			drupal_goto('circuit-simulation-project/manage-proposal');
		return;
		} //rrmdir_project($proposal_id) == TRUE
	} //$form_state['values']['delete_proposal'] == 1
	/* update proposal */
	$v = $form_state['values'];
	$project_title = $v['project_title'];
	$proposar_name = $v['name_title'] . ' ' . $v['contributor_name'];
	$university = $v['university'];
	$directory_names = _df_dir_name($project_title, $proposar_name);
	if (DF_RenameDir($proposal_id, $directory_names))
	{
		$directory_name = $directory_names;
	} //LM_RenameDir($proposal_id, $directory_names)
	else
	{
		return;
	}
	$str = substr($proposal_data->samplefilepath,strrpos($proposal_data->samplefilepath, '/'));
	$resource_file =ltrim($str, '/');
	$samplefilepath = $directory_name . '/' . $resource_file;
	$query = "UPDATE esim_circuit_simulation_proposal SET 
				name_title=:name_title,
				contributor_name=:contributor_name,
				university=:university,
				project_guide_name=:project_guide_name,
				project_guide_email_id=:project_guide_email_id,
				city=:city,
				pincode=:pincode,
				state=:state,
				project_title=:project_title,
				description=:description,
				operating_system=:operating_system,
				reference=:reference,
				directory_name=:directory_name ,
				samplefilepath=:samplefilepath
				WHERE id=:proposal_id";
	$args = array(
		':name_title' => $v['name_title'],
		':contributor_name' => $v['contributor_name'],
		':university' => $v['university'],
		':project_guide_name' => $v['project_guide_name'],
		':project_guide_email_id' => $v['project_guide_email_id'],
		':city' => $v['city'],
		':pincode' => $v['pincode'],
		':state' => $v['all_state'],
		':project_title' => $project_title,
		':description' => $v['description'],
		':operating_system' => $v['operating_system'],
		':reference' => $v['reference'],
		':directory_name' => $directory_name,
		':samplefilepath' => $samplefilepath,
		':proposal_id' => $proposal_id
	);
	$result = db_query($query, $args);
	drupal_set_message(t('Proposal Updated'), 'status');
}