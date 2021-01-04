<?php
// eSim Circuit Simulation display completed proposals
function circuit_simulation_completed_proposals_all()
{
	$output = "";
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('approval_status', 3);
	$query->orderBy('actual_completion_date', 'DESC');
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
	return $output;
}
// eSim Circuit Simulation display in progress proposals
function circuit_simulation_progress_all()
{
	$page_content = "";
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('approval_status', 1);
	$query->condition('is_completed', 0);
	$result = $query->execute();
	if ($result->rowCount() == 0)
	{
		$page_content .= "Work is in progress for the following circuit simulation under Circuit Simulation Project<hr>";
	} //$result->rowCount() == 0
	else
	{
		$page_content .= "Work is in progress for the following circuit simulation under Circuit Simulation Project<hr>";
		$preference_rows = array();
		$i = 1;
		while ($row = $result->fetchObject())
		{
			$approval_date = date("Y", $row->approval_date);
			$preference_rows[] = array(
				$i,
				$row->project_title,
				$row->contributor_name,
				$row->university,
				$approval_date
			);
			$i++;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No',
			'Circuit Simulation Project',
			'Contributor Name',
			'Institute',
			'Year'
		);
		$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $page_content;
}

function circuit_simulation_completed_pspice_to_kicad()
{
	$output = "";
	$query = db_select('pspice_to_kicad_circuits');
	$query->fields('pspice_to_kicad_circuits');
	//$query->condition('is_completed', 1);
	$result = $query->execute();
	
	//var_dump($esim_project_abstract);die;
	if ($result->rowCount() == 0)
	{
		$output .= "<h4 dir='ltr'><span style='color:#008000'><strong>PSpice to KiCad Converter</strong></span></h4>

<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>This feature converts a schematic file created using PSpice&reg; to KiCad format. The converted schematic file is compatible with KiCad for PCB layout. You can also </span><span style='background-color:#fcfcfc; color:#000000; font-family:times new roman; font-size:12pt'>create a netlist and simulate using Ngspice.&nbsp;</span></p>

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

<p dir='ltr'><span style='background-color:transparent; color:#000000; font-family:times new roman; font-size:12pt'>This feature converts a schematic file created using PSpice&reg; to KiCad format. The converted schematic file is compatible with KiCad for PCB layout. You can also </span><span style='background-color:#fcfcfc; color:#000000; font-family:times new roman; font-size:12pt'>create a netlist and simulate using Ngspice.&nbsp;</span></p>

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
	return $output;
}