<?php
// display completed circuits
function completed_circuit_proposals_all_idea1()
{
	$output = "";
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('approval_status', 3);
	$query->orderBy('actual_completion_date', 'DESC');
	$query->range(0, 10);
	//$query->condition('is_completed', 1);
	$result = $query->execute();
	
	//var_dump($esim_project_abstract);die;
	if ($result->rowCount() == 0)
	{
		$output .= "Work has been completed for the following circuit simulation. We welcome your contributions." ."<hr>";

	} //$result->rowCount() == 0
	else
	{
		$output .= "Work has been completed for the following circuit simulation. We welcome your contributions." . "<hr>";
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$proposal_id = $row->id;
			$query1 = db_select('esim_circuit_simulation_submitted_abstracts_file');
			$query1->fields('esim_circuit_simulation_submitted_abstracts_file');
			$query1->condition('file_approval_status', 1);
			$query1->condition('proposal_id', $proposal_id);
			$esim_project_files = $query1->execute();
			$esim_project_abstract = $esim_project_files->fetchObject();
			$completion_date = date("Y", $row->actual_completion_date);
			$preference_rows[] = array(
				$i,
				l($row->project_title, "circuit-simulation-project/esim-circuit-simulation-run/" . $row->id),
				$row->contributor_name,
				$row->university,
				$completion_date
			);
			$i++;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No',
			'Circuit Simulation Project',
			'Contributor Name',
			'University / Institute',
			'Year of Completion'
		);
		$output .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	$output .= " ".l("More Completed Circuits","/circuit-simulation-project/completed-circuits"). " " ."<h4>";
	return $output;
}


function completed_circuit_pspice_to_kicad_idea1()
{
	$output = "";
	$query = db_select('pspice_to_kicad_circuits');
	$query->fields('pspice_to_kicad_circuits');
	$query->range(0, 10);
	//$query->condition('is_completed', 1);
	$result = $query->execute();
	
	//var_dump($esim_project_abstract);die;
	if ($result->rowCount() == 0)
	{
		$output .= "<h4 dir='ltr'><span style='color:#008000'><strong>PSpice to KiCad Converter</strong></span></h4>

<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>This feature converts a schematic file created using PSpice&reg; to KiCad format. The converted schematic file is compatible with KiCad for PCB layout. You can also </span><span style='background-color:#fcfcfc; color:#000000; font-family:times new roman; font-size:12pt'>create a netlist and simulate using Ngspice. The source code for this converter is available <span style='text-decoration: underline;'><a href='https://github.com/FOSSEE/eSim_PSpice_to_KiCad_Python_Parser' target='_blank'>here</a>.</span></p>

<p dir='ltr'><span style='color:#008000'><strong>How to convert?</strong></span></p>

<ol>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>Login to eSim website</span><a href='https://esim.fossee.in/' style='text-decoration:none;' target='_blank'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'> </span><u>here</u></a><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>Upload the PSpice Schematic files by clicking <a href='https://esim.fossee.in/pspice-to-kicad/add' style='text-decoration:none;' target='_blank'><u>here</u></a><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>. </span><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>&nbsp;&nbsp;&nbsp; </span></span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>The PSpice schematic should be in standard format. Refer <a href='https://static.fossee.in/esim/manuals/trafo3ph_Dyn11.sch' target='_blank'>here</a>, for a few PSpice schematic samples.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>Your files will be converted to the KiCad schematic format. Once the conversion is complete, you will be intimated through your registered email id.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>You can use the KiCad schematic further for Simulation and for PCB generation. Follow the instructions <a href='https://static.fossee.in/esim/manuals/Instructions_PSpice_to_KiCad.pdf' target='_blank'>here</a>.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'> Converted files after verification are made available for download in the table below.</p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>  We welcome your contributions to this page</p>
	</li>
</ol>" . "<hr>";

	} //$result->rowCount() == 0
	else
	{
		$output .= "<h4 dir='ltr'><span style='color:#008000'><strong>PSpice to KiCad Converter</strong></span></h4>

<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>This feature converts a schematic file created using PSpice&reg; to KiCad format. The converted schematic file is compatible with KiCad for PCB layout. You can also </span><span style='background-color:#fcfcfc; color:#000000; font-family:times new roman; font-size:12pt'>create a netlist and simulate using Ngspice. The source code for this converter is available <span style='text-decoration: underline;'><a href='https://github.com/FOSSEE/eSim_PSpice_to_KiCad_Python_Parser' target='_blank'>here</a>.</span></p>

<p dir='ltr'><span style='color:#008000'><strong>How to convert?</strong></span></p>

<ol>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>Login to eSim website</span><a href='https://esim.fossee.in/' style='text-decoration:none;' target='_blank'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'> </span><u>here</u></a><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>Upload the PSpice Schematic files by clicking <a href='https://esim.fossee.in/pspice-to-kicad/add' style='text-decoration:none;' target='_blank'><u>here</u></a><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>. </span><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>&nbsp;&nbsp;&nbsp; </span></span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>The PSpice schematic should be in standard format. Refer <a href='https://static.fossee.in/esim/manuals/trafo3ph_Dyn11.sch' target='_blank'>here</a>, for a few PSpice schematic samples.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>Your files will be converted to the KiCad schematic format. Once the conversion is complete, you will be intimated through your registered email id.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>You can use the KiCad schematic further for Simulation and for PCB generation. Follow the instructions <a href='https://static.fossee.in/esim/manuals/Instructions_PSpice_to_KiCad.pdf' target='_blank'>here</a>.</span></p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'> Converted files after verification are made available for download in the table below.</p>
	</li>
	<li dir='ltr'>
	<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>  We welcome your contributions to this page</p>
	</li>
</ol>" . "<hr>";
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			/*$proposal_id = $row->id;
			$query1 = db_select('esim_circuit_simulation_submitted_abstracts_file');
			$query1->fields('esim_circuit_simulation_submitted_abstracts_file');
			$query1->condition('file_approval_status', 1);
			$query1->condition('proposal_id', $proposal_id);
			$esim_project_files = $query1->execute();
			$esim_project_abstract = $esim_project_files->fetchObject();
			$completion_date = date("Y", $row->actual_completion_date);*/
			$preference_rows[] = array(
				$i,
				l($row->name_of_circuit, 'https://static.fossee.in/esim/converters/pspicetokicad_PAGE_all/' . $row->filename . '.tar.gz')
				//$row->university,
				//$completion_date
			);
			$i++;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No',
			'Name of the Circuit',
			//'Download'
			//'Contributor Name',
			//'University / Institute',
			//'Year of Completion'
		);
		$output .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	$output .= " ".l("More Completed pspice_to_kicad Circuits","/circuit-simulation-project/completed-circuits/pspice-to-kicad"). " " ."<h4>";
	return $output;
}


function completed_circuits_hackathon_submissions_idea1()
{
    $output = "";
    $query = db_select('hackathon_completed_circuits');
    $query->fields('hackathon_completed_circuits'); 
	$query->range(0, 10);
    $result = $query->execute();
    //var_dump($result->rowCount());die;
    $i = 1;
    $output .= "FOSSEE Project in collaboration with VLSI System Design (VSD) Corp. Pvt. Ltd and the Ministry of Education, Govt. of India conducted a 2-weeks high intensity eSim Circuit Design and Simulation Marathon using Skywater 130nm technology, a fully open source process design kit. Close to 3000+ students from all over India participated in this Marathon and close to 200+ students completed this marathon with brilliant circuit design ideas. The following participants have successfully completed designing the circuits. More details about this event can be found here: <a href='https://hackathon.fossee.in/esim' target='_blank'>https://hackathon.fossee.in/esim</a>.<hr>";
    $preference_rows = array();
    while($row_completed_circuits = $result->fetchObject())
    {        
        $query_pro = db_select('hackathon_literature_survey');
        $query_pro->fields('hackathon_literature_survey');
        $query_pro->condition('id', $row_completed_circuits->literature_survey_id); 
		$query->range(0, 10);
        $result_pro = $query_pro->execute();
        while ($row = $result_pro->fetchObject())
        {
            //$approval_date = date("Y", $row->approval_date);
            $preference_rows[] = array(
                $i,
                l($row->circuit_name, 'hackathon-submission/download/completed-circuit/' . $row_completed_circuits->literature_survey_id),
                wordwrap($row->participant_name, 10, "\n", false),
                $row->institute,
                //$approval_date
            );
            
        } //$row = $result->fetchObject()
        $i++;
    }
    $preference_header = array(
            'No',
            'Circuit Name',
            'Participant Name',
            'Institute',
        );
        $output .= theme('table', array(
            'header' => $preference_header,
            'rows' => $preference_rows
        ));
    $output .= " ".l("More Completed Marathon 2021 Circuits","/hackathon/completed-circuits"). " " ."<h4>";
    return $output;
}

function completed_circuits_mscd_display_final_submissions_idea1()
{
    $my_submission_rows = array();
    $query = db_select('mixed_signal_marathon_final_submission');
    $query->fields('mixed_signal_marathon_final_submission');
    $query->orderBy('id', 'DESC');
	$query->range(0, 10);
    $my_submissions_q = $query->execute();
    $i = 1;
    $output .= "Work has been completed for the following esim Marathon 2022." ."<hr>";
    while ($my_submissions_data = $my_submissions_q->fetchObject())
    {
        $query = db_select('mixed_signal_marathon_literature_survey');
        $query->fields('mixed_signal_marathon_literature_survey');
        $query->condition('id',$my_submissions_data->literature_survey_id);
        $query->orderBy('id', 'DESC');
        $final_submissions_q = $query->execute();
        $final_submission_data = $final_submissions_q->fetchObject();
        $action =  l('View', 'mixed-signal-design-marathon/view/final-submission/' . $final_submission_data->id);  
        $my_submission_rows[$my_submissions_data->id] = array(
        	$i,
            date('d-m-Y', $my_submissions_data->creation_date),
            l($final_submission_data->participant_name, 'user/' . $final_submission_data->uid),
            $final_submission_data->circuit_name,
            $action
        );
        $i++;
    } //$pending_data = $pending_q->fetchObject()
    /* check if there are any pending submissions */
    if (!$my_submission_rows)
    {
        drupal_set_message(t('There are no active submissions'), 'status');
        return '';
    } //!$pending_rows
    $my_submission_header = array(
    	'S.No',
        'Date of Submission',
        'Name',
        'Circuit Name',
        'Final Submission'
    );
    //$output = theme_table($pending_header, $pending_rows);
    $output .= theme('table', array(
        'header' => $my_submission_header,
        'rows' => $my_submission_rows
    ));
    $output .= " ".l("More Completed Marathon 2022 Circuits","/mixed-signal-design-marathon/all-submissions"). " " ."<h4>";
    return $output;
}